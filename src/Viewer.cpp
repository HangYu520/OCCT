#include "Viewer.hpp"
#include "IO.hpp"

// --- OpenCASCADE 到 VTK 的桥接库 ---
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkOCC_Shape.hxx>

// --- VTK 核心库 ---
#include <vtkAutoInit.h>
#include <vtkPolyDataMapper.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkCallbackCommand.h>
#include <vtkTextProperty.h>

#include <string>
#include <cstdlib>
#include <iostream>
#include <numeric>

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

    // --- 初始化文本 Actor (日志消息窗口) ---
    m_textActor = vtkSmartPointer<vtkTextActor>::New();
    m_textActor->GetTextProperty()->SetFontSize(24);
    m_textActor->GetTextProperty()->SetColor(0.0, 1.0, 0.0); // 绿色文字
    m_textActor->GetTextProperty()->SetFontFamilyToCourier();
    m_textActor->SetPosition(10, 10); // 左下角位置
    m_renderer->AddViewProp(m_textActor);

    Log("Viewer initialized. (C: Random Color | Ctrl+S: Save STEP)");

    // --- 添加键盘监听 ---
    auto keyPressClick = vtkSmartPointer<vtkCallbackCommand>::New();
    keyPressClick->SetCallback(Viewer::KeyPressCallbackFunction);
    keyPressClick->SetClientData(this); 
    m_interactor->AddObserver(vtkCommand::KeyPressEvent, keyPressClick);
}

void Viewer::Log(const std::string& msg) {
    std::cout << msg << std::endl; // 同时输出到控制台
    m_logMessages.push_back(msg);
    if (m_logMessages.size() > m_maxLogLines) {
        m_logMessages.erase(m_logMessages.begin());
    }
    UpdateTextDisplay();
    if (m_renderWindow) {
        m_renderWindow->Render();
    }
}

void Viewer::UpdateTextDisplay() {
    if (m_logMessages.empty()) return;
    
    std::string fullText = "";
    for (const auto& msg : m_logMessages) {
        fullText += msg + "\n";
    }
    m_textActor->SetInput(fullText.c_str());
}

void Viewer::Show(const TopoDS_Shape& shape) {
    if (shape.IsNull()) {
        Log("Warning: Cannot show a null shape.");
        return;
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
}

void Viewer::Start() {
    m_renderWindow->Render();
    m_interactor->Start();
}

void Viewer::KeyPressCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData) {
    auto* iren = static_cast<vtkRenderWindowInteractor*>(caller);
    auto* viewer = static_cast<Viewer*>(clientData);
    
    std::string key = iren->GetKeySym();
    
    if (key == "c" || key == "C") {
        if (viewer->m_currentActor) {
            double r = (double)rand() / RAND_MAX;
            double g = (double)rand() / RAND_MAX;
            double b = (double)rand() / RAND_MAX;
            viewer->m_currentActor->GetProperty()->SetColor(r, g, b);
            viewer->Log("Randomized object color.");
        }
    } else if ((key == "s" || key == "S") && iren->GetControlKey()) {
        if (!viewer->m_currentShape.IsNull()) {
            viewer->Log("Saving shape to save.stp...");
            if (IO::SaveStep(viewer->m_currentShape, "save.stp")) {
                viewer->Log("Successfully saved to save.stp");
            } else {
                viewer->Log("Error: Failed to save to save.stp");
            }
        }
    }
}
