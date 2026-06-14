#version 460 core
out vec4 FragColor;


uniform sampler2D sampler;
uniform sampler2D opacityMaskSampler;

uniform float width;
uniform float height;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform vec3 ambientColor;

//相机位置
uniform vec3 cameraPosition;

//透明度
uniform float opacity;


uniform float shiness;

struct DirectionalLight{
	vec3 direction;
	vec3 color;
	float specularIntensity;
};

struct SpotLight{
	vec3 position;
	vec3 targetDirection;
	vec3 color;
	float innerLine;
	float outerLine;
	float specularIntensity;
};

struct PointLight{
	vec3 position;
	vec3 color;
	float specularIntensity;
	
	float k2;
	float k1;
	float kc;
};

uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;

#define POIN_LIGHT_NUM 4
uniform PointLight pointLights[POIN_LIGHT_NUM];

//漫反射光照计算函数
vec3 calculateDiffuse(vec3 lightColor,vec3 objectColor,vec3 lightDir,vec3 normal){//约定传入的矢量必须进行归一化
	//float diffuse = max(dot(normal, lightDir), 0.0);      //关于max和clamp的使用？关于lightDir的方向？
	float diffuse = clamp(dot(lightDir,normal), 0.0, 1.0);
	vec3 diffuseColor = lightColor * objectColor * diffuse;
	return diffuseColor;
}

//计算镜面反射光照
vec3 calculateSpecular(vec3 lightColor,vec3 lightDir, vec3 normal, vec3 viewDir, float intensity){
	//剔除背面光（防止背面高光）
	float dotResult = dot(normal, lightDir);          // ★修改：用 dot(N, L)
	float flag = step(0.0, dotResult);

	// ★修改3：reflect 的入射方向必须是 -L（GLSL 定义）
	vec3 lightReflect = normalize(reflect(-lightDir, normal));  // ★核心修改

	// ★修改4：dot(R, V) 仍然按统一方向直接计算
	float specular = max(dot(lightReflect, viewDir), 0.0);        // ★修改：移除负号
	specular = pow(specular, shiness);

	//float specularMask = texture(specularMaskSampler, uv).r;//并非所有模型都有遮罩
	vec3 specularColor = lightColor * specular * flag * intensity ;

	return specularColor;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir){
	vec3 objectColor = texture(sampler, uv).xyz;
	//vec3 lightDir = normalize(worldPosition - light.position);

	// ★修复 1：统一 L = Frag → Light
    vec3 lightDir = normalize(light.position - worldPosition);
	
	vec3 targetDir = normalize(light.targetDirection);

	//计算spotLight的照射范围

    float cGamma = dot(lightDir, targetDir);               // ★修复 dot 方向

	float intensity = clamp((cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0, 1.0);

	//diffuse
	// dot(N, L) —— 使用统一方向规则
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

	//specular计算
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity); 

	return (diffuseColor + specularColor) * intensity;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir){
	 //计算光照通用数据
	vec3 objectColor = texture(sampler, uv).rgb;
	vec3 lightDir = normalize(light.direction); 

	//diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

	//specular计算
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity); 

	return diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir){
vec3 objectColor = texture(sampler, uv).rgb;
vec3 lightDir = normalize(light.position - worldPosition); //注意这里的方向可能有问题
	//计算距离衰减
	float distance = length(light.position - worldPosition);
	float attenuation = 1.0 / (light.kc + light.k1 * distance + light.k2 * (distance * distance));
	//diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);
	//specular计算
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity); 
	return (diffuseColor + specularColor) * attenuation;
}

void main()
{
	vec3 result = vec3(0.0);

	 //计算光照通用数据
	vec3 objectColor = texture(sampler, uv).rgb;

	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(spotLight.position - worldPosition); 
	vec3 viewDir = normalize(cameraPosition - worldPosition);
	vec3 targetDirN = normalize(spotLight.targetDirection);

	result += calculateSpotLight(spotLight, normalN, viewDir);
	result += calculateDirectionalLight(directionalLight, normalN, viewDir);

	for(int i = 0; i < POIN_LIGHT_NUM; i++){
	result += calculatePointLight(pointLights[i], normalN, viewDir);
}
	//环境光计算 
	vec3 ambientTerm = ambientColor * objectColor;

	vec3 finalColor = result + ambientTerm;

	float alpha = texture(opacityMaskSampler, uv).r;
	
/*
	//临时使用
	float flag = step(800,gl_FragCoord.x);
	vec3 blendColor = mix(vec3(1.0,0.0,0.0),vec3(0.0,0.0,1.0),flag);
	finalColor *= blendColor;
*/
	FragColor = vec4(finalColor, alpha * opacity );


}