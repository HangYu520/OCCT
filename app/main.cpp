#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>

// --- OpenCASCADE 到 VTK 的桥接库 ---
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkOCC_Shape.hxx>

// --- VTK 核心库 ---
#include <vtkAutoInit.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h> // 用于设置 Actor 属性（颜色、材质）
#include <vtkCallbackCommand.h> // 用于键盘回调监听

// 这是一个简单的回调函数，当有键盘事件发生时，VTK 会调用它
void KeyPressCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData) 
{
    vtkRenderWindowInteractor* iren = static_cast<vtkRenderWindowInteractor*>(caller);
    vtkActor* actor = static_cast<vtkActor*>(clientData);
    
    // 获取按下的键
    std::string key = iren->GetKeySym();
    
    if (key == "c" || key == "C") {
        // 如果按下 'c'，随机改变物体颜色
        double r = (double)rand() / RAND_MAX;
        double g = (double)rand() / RAND_MAX;
        double b = (double)rand() / RAND_MAX;
        actor->GetProperty()->SetColor(r, g, b);
        iren->Render(); // 重新渲染界面
    }
}

int main() 
{
    // 1. 创建 OCCT 长方体
    BRepPrimAPI_MakeBox mkbox(10.0, 20.0, 30.0);
    TopoDS_Shape box = mkbox.Shape();

    // 2. OCCT 形状转 VTK 数据
    IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(box);
    vtkSmartPointer<IVtkTools_ShapeDataSource> aDataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
    aDataSource->SetShape(aShapeImpl);

    // 3. 构建渲染管线
    vtkSmartPointer<vtkPolyDataMapper> aMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    aMapper->SetInputConnection(aDataSource->GetOutputPort());

    vtkSmartPointer<vtkActor> anActor = vtkSmartPointer<vtkActor>::New();
    anActor->SetMapper(aMapper);
    anActor->GetProperty()->SetColor(0.7, 0.7, 0.7); // 初始颜色：浅灰色

    // 4. 设置渲染器和窗口
    vtkSmartPointer<vtkRenderer> aRenderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> aRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    aRenderWindow->AddRenderer(aRenderer);
    aRenderWindow->SetWindowName("Learn-OCCT");
    aRenderWindow->SetSize(800, 600);

    vtkSmartPointer<vtkRenderWindowInteractor> anInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    anInteractor->SetRenderWindow(aRenderWindow);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> aStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    anInteractor->SetInteractorStyle(aStyle);

    // --- 添加键盘监听 ---
    vtkSmartPointer<vtkCallbackCommand> keyPressClick = vtkSmartPointer<vtkCallbackCommand>::New();
    keyPressClick->SetCallback(KeyPressCallbackFunction);
    keyPressClick->SetClientData(anActor); // 将 Actor 传递给回调函数，以便修改它
    anInteractor->AddObserver(vtkCommand::KeyPressEvent, keyPressClick);

    // 5. 显示并启动
    aRenderer->AddActor(anActor);
    aRenderer->SetBackground(0.1, 0.1, 0.1); // 背景色：深灰色

    aRenderWindow->Render();
    anInteractor->Start();

    return 0;  
}