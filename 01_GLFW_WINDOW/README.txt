2026年3月31日16:55:23
已完成析构的基本整理。
存在两个问题：
1，material子类均未做内存释放
2，共享texture缺少unit，也缺少释放时需要的判断tag

完整日志
Application 构造函数
OpenGL版本号：4.6.0 - Build 32.0.101.6790
[Ctor] Renderer 00000278B41BD0C0
[Ctor] Shader 00000278B9C16290 vs=assets/shaders/phong.vert fs=assets/shaders/phong.frag
[Ctor] Shader 00000278B9C14170 vs=assets/shaders/white.vert fs=assets/shaders/white.frag
[Ctor] Shader 00000278B9B88650 vs=assets/shaders/depth.vert fs=assets/shaders/depth.frag
[Ctor] Shader 00000278B9C242A0 vs=assets/shaders/phongOpacityMask.vert fs=assets/shaders/phongOpacityMask.frag
[Ctor] Shader 00000278B9B91B40 vs=assets/shaders/screen.vert fs=assets/shaders/screen.frag
[Ctor] Shader 00000278B9B88210 vs=assets/shaders/cube.vert fs=assets/shaders/cube.frag
[Ctor] Shader 00000278B9B91A40 vs=assets/shaders/phongEnv.vert fs=assets/shaders/phongEnv.frag
[Ctor] Object 00000278B9A296F0
[Ctor] Object 00000278B93CA430
[Ctor] Object 00000278B41BDC00
[Ctor] Object 00000278B41BD600
[Ctor] Object 00000278B41BD240
[Ctor] Object 00000278B41BE140
[Ctor-Begin] Texture(cubemap) 00000278B9A84460 unit=0 faces=6
[Ctor-End] Texture(cubemap) 00000278B9A84460 id=1
[Ctor] Geometry(default) 00000278B9A90760
[Ctor] Material 00000278B93D33F0
[Ctor] CubeMaterial 00000278B93D33F0
[Ctor] Object 00000278B93D2E70
[Ctor] Mesh 00000278B93D2E70
[Ctor-Begin] Texture(cubemap) 00000278B9A84AF0 unit=1 faces=6
[Ctor-End] Texture(cubemap) 00000278B9A84AF0 id=2
[Ctor] Geometry(default) 00000278B9A90B80
[Ctor] Material 00000278B41BE2C0
[Ctor] PhongEnvMaterial 00000278B41BE2C0
[Ctor-Begin] Texture(file) 00000278B9A84A00 path=assets/textures/earth.png unit=0
[Ctor-End] Texture(file) 00000278B9A84A00 id=3
[Ctor] Object 00000278B93D3130
[Ctor] Mesh 00000278B93D3130
[Dtor] Object 00000278B41BDC00
[Dtor] Object 00000278B41BD600
[Dtor] Object 00000278B41BD240
[Dtor] Object 00000278B41BE140
[Dtor] Object 00000278B93CA430
[Dtor] Mesh 00000278B93D2E70
[Dtor] Geometry 00000278B9A90760 vao=1 posVbo=1 uvVbo=0 normalVbo=0 ebo=4
[Dtor] CubeMaterial 00000278B93D33F0
[Dtor] Material 00000278B93D33F0
[Dtor] Object 00000278B93D2E70
[Dtor] Mesh 00000278B93D3130
[Dtor] Geometry 00000278B9A90B80 vao=2 posVbo=5 uvVbo=0 normalVbo=0 ebo=8
[Dtor] PhongEnvMaterial 00000278B41BE2C0
[Dtor] Material 00000278B41BE2C0
[Dtor] Object 00000278B93D3130
[Dtor] Object 00000278B9A296F0
[Dtor] Renderer 00000278B41BD0C0
[Dtor] Shader 00000278B9C16290 program=3
[Dtor] Shader 00000278B9C14170 program=6
[Dtor] Shader 00000278B9B88650 program=9
[Dtor] Shader 00000278B9C242A0 program=12
[Dtor] Shader 00000278B9B91B40 program=15
[Dtor] Shader 00000278B9B88210 program=18
[Dtor] Shader 00000278B9B91A40 program=21
[Texture] clearCache begin, count=0
[Texture] clearCache end