# Scene/Entity/Component系统使用指南

## 系统架构

### 核心类

1. **Component** - 组件基类
   - 生命周期：OnCreate(), OnUpdate(), OnDestroy()
   - 启用/禁用控制

2. **Transform** - 变换组件（每个Entity必有）
   - 位置、旋转（四元数）、缩放
   - 局部和世界坐标系
   - 父子层级关系支持

3. **Entity** - 实体类
   - 管理组件集合（AddComponent, GetComponent, RemoveComponent）
   - 父子层级关系（SetParent, AddChild, RemoveChild）
   - Update循环传递

4. **Scene** - 场景管理器
   - Entity创建和管理
   - 主相机管理
   - 场景图遍历和渲染

5. **MeshRenderer** - 网格渲染组件
   - Shader、Geometry、Texture管理
   - 自动MVP矩阵计算

6. **CameraComponent** - 相机组件
   - 集成Camera类
   - 视图和投影矩阵生成

## 使用示例

```cpp
#include "src/Scene/Scene.h"
#include "src/Scene/Entity.h"
#include "src/Scene/MeshRenderer.h"
#include "src/Scene/CameraComponent.h"

// 创建场景
auto scene = std::make_shared<Scene>("MyScene");

// 创建相机实体
auto cameraEntity = scene->CreateEntity("MainCamera");
cameraEntity->GetTransform()->SetPosition(glm::vec3(0, 0, 5));
auto camera = cameraEntity->AddComponent<CameraComponent>();
scene->SetMainCamera(camera);

// 创建渲染对象
auto cubeEntity = scene->CreateEntity("Cube");
cubeEntity->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
cubeEntity->GetTransform()->SetScale(glm::vec3(1, 1, 1));

auto renderer = cubeEntity->AddComponent<MeshRenderer>();
renderer->SetShader(myShader);
renderer->SetGeometry(myGeometry);
renderer->SetTexture(myTexture);

// 在Application中使用
app.SetActiveScene(scene);

// 场景会自动Update和Render
```

## 层级关系示例

```cpp
// 创建父子关系
auto parent = scene->CreateEntity("Parent");
auto child = scene->CreateEntity("Child");
child->SetParent(parent);

// 子节点的Transform会继承父节点变换
parent->GetTransform()->SetPosition(glm::vec3(1, 0, 0));
child->GetTransform()->SetPosition(glm::vec3(0, 1, 0)); // 世界坐标为(1, 1, 0)
```

## 组件扩展

创建自定义组件：

```cpp
class MyComponent : public Component {
public:
    std::string GetTypeName() const override { return "MyComponent"; }
    
    void OnCreate() override {
        // 组件创建时调用
    }
    
    void OnUpdate(float deltaTime) override {
        // 每帧更新
    }
    
    void OnDestroy() override {
        // 组件销毁时调用
    }
};

// 使用
entity->AddComponent<MyComponent>();
```

## 目录结构

```
src/Scene/
├── Component.h          # 组件基类
├── Transform.h/cpp      # 变换组件
├── Entity.h/cpp         # 实体类
├── Scene.h/cpp          # 场景管理
├── MeshRenderer.h/cpp   # 网格渲染组件
└── CameraComponent.h/cpp # 相机组件
```
