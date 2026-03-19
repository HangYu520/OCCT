#include "Viewer.hpp"
#include <vtkPolyDataMapper.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>
#include <vtkTextProperty.h>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkOCC_Shape.hxx>
#include <iostream>
// --- custom ---
#include "io/IO.hpp"

Viewer::Viewer() {
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_renderWindow->AddRenderer(m_renderer);
    m_renderWindow->SetWindowName("Learn-OCCT Viewer");
    m_renderWindow->SetSize(800, 600);

    m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_interactor->SetRenderWindow(m_renderWindow);

    auto aStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    m_interactor->SetInteractorStyle(aStyle);

    m_renderer->SetBackground(0.1, 0.1, 0.1);

    Log("Viewer initialized. (C: Color | R: Reset Shape | Ctrl+S: Save Step)");

    // --- 添加键盘监听 ---
    auto keyPressClick = vtkSmartPointer<vtkCallbackCommand>::New();
    keyPressClick->SetCallback(Viewer::KeyPressCallbackFunction);
    keyPressClick->SetClientData(this); 
    m_interactor->AddObserver(vtkCommand::KeyPressEvent, keyPressClick);
}

void Viewer::Log(const std::string& msg, LogLevel level) {
    std::string prefix;
    switch (level) {
        case LogLevel::Info:    prefix = "[INFO] "; break;
        case LogLevel::Warning: prefix = "[WARN] "; break;
        case LogLevel::Error:   prefix = "[ERROR] "; break;
    }

    std::string fullMsg = prefix + msg;

    // 1. 控制台同步打印
    std::cout << fullMsg << std::endl;

    // 2. 创建新的文本 Actor
    auto actor = vtkSmartPointer<vtkTextActor>::New();
    actor->SetInput(fullMsg.c_str());
    actor->GetTextProperty()->SetFontSize(m_fontSize);
    actor->GetTextProperty()->SetFontFamilyToCourier();
    actor->GetTextProperty()->SetBold(1);

    // 3. 根据级别设置颜色
    switch (level) {
        case LogLevel::Info:
            actor->GetTextProperty()->SetColor(0.0, 1.0, 0.0); // 绿色
            break;
        case LogLevel::Warning:
            actor->GetTextProperty()->SetColor(1.0, 1.0, 0.0); // 黄色
            break;
        case LogLevel::Error:
            actor->GetTextProperty()->SetColor(1.0, 0.0, 0.0); // 红色
            break;
    }

    // 4. 管理历史消息
    m_logActors.push_back(actor);
    m_renderer->AddViewProp(actor);

    if (m_logActors.size() > m_maxLogLines) {
        m_renderer->RemoveViewProp(m_logActors.front());
        m_logActors.erase(m_logActors.begin());
    }

    // 5. 更新显示布局
    UpdateLogDisplay();

    if (m_renderWindow) {
        m_renderWindow->Render();
    }
}

void Viewer::UpdateLogDisplay() {
    // 最新的一条显示在最下面 (Y = 10)
    for (size_t i = 0; i < m_logActors.size(); ++i) {
        size_t reverseIdx = m_logActors.size() - 1 - i;
        m_logActors[reverseIdx]->SetPosition(10, 10 + i * m_lineSpacing);
    }
}

void Viewer::Clear() {
    if (m_currentActor) {
        m_renderer->RemoveActor(m_currentActor);
        m_currentActor = nullptr;
    }
    m_currentShape.Nullify();
    if (m_renderWindow) {
        m_renderWindow->Render();
    }
}

void Viewer::ResetShape() {
    if (m_initialShape.IsNull()) {
        Log("No initial shape to reset to.", LogLevel::Warning);
        return;
    }
    Log("Resetting to initial shape...");
    Show(m_initialShape);
}

void Viewer::Show(const TopoDS_Shape& shape) {
    if (shape.IsNull()) {
        Log("Cannot show a null shape.", LogLevel::Warning);
        return;
    }

    // --- 如果这是第一次 Show，记录为初始形状 ---
    if (m_initialShape.IsNull()) {
        m_initialShape = shape;
    }

    // 如果当前已有形状，先移除它
    if (m_currentActor) {
        m_renderer->RemoveActor(m_currentActor);
    }

    m_currentShape = shape;

    // 1. OCCT 形状转 VTK 数据
    IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(shape);
    auto aDataSource = vtkSmartPointer<IVtkTools_ShapeDataSource>::New();
    aDataSource->SetShape(aShapeImpl);

    // 2. 构建渲染管线
    auto aMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    aMapper->SetInputConnection(aDataSource->GetOutputPort());

    auto anActor = vtkSmartPointer<vtkActor>::New();
    anActor->SetMapper(aMapper);
    anActor->GetProperty()->SetColor(0.7, 0.7, 0.7);

    m_currentActor = anActor;

    // 3. 添加到渲染器
    m_renderer->AddActor(anActor);
    m_renderer->ResetCamera();

    if (m_renderWindow) {
        m_renderWindow->Render();
    }
}

void Viewer::Start() {
    m_renderWindow->Render();
    m_interactor->Start();
}

void Viewer::KeyPressCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData) {
    auto* iren = static_cast<vtkRenderWindowInteractor*>(caller);
    auto* viewer = static_cast<Viewer*>(clientData);
    
    std::string key = iren->GetKeySym();
    bool ctrlPressed = iren->GetControlKey() != 0;

    if (ctrlPressed && (key == "s" || key == "S")) {
        if (!viewer->m_currentShape.IsNull()) {
            MSG res = IO::SaveStep(viewer->m_currentShape, "save.stp");
            viewer->Log(res.msg, res.level);
        } else {
            viewer->Log("No shape to save.", LogLevel::Warning);
        }
    } else if (key == "c" || key == "C") {
        if (viewer->m_currentActor) {
            double r = (double)rand() / RAND_MAX;
            double g = (double)rand() / RAND_MAX;
            double b = (double)rand() / RAND_MAX;
            viewer->m_currentActor->GetProperty()->SetColor(r, g, b);
            viewer->Log("Randomized object color.");
        }
    } else if (key == "r" || key == "R") {
        viewer->ResetShape();
    }
}
