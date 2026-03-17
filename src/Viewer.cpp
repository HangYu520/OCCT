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

#include <string>
#include <cstdlib>
#include <iostream>

Viewer::Viewer() {
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_renderWindow->AddRenderer(m_renderer);
    m_renderWindow->SetWindowName("Learn-OCCT Viewer (C: Random Color | S: Save STEP)");
    m_renderWindow->SetSize(800, 600);

    m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_interactor->SetRenderWindow(m_renderWindow);

    auto aStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    m_interactor->SetInteractorStyle(aStyle);

    m_renderer->SetBackground(0.1, 0.1, 0.1);

    // 在构造时添加键盘监听，将 this 传给回调
    auto keyPressClick = vtkSmartPointer<vtkCallbackCommand>::New();
    keyPressClick->SetCallback(Viewer::KeyPressCallbackFunction);
    keyPressClick->SetClientData(this); 
    m_interactor->AddObserver(vtkCommand::KeyPressEvent, keyPressClick);
}

void Viewer::Show(const TopoDS_Shape& shape) {
    if (shape.IsNull()) return;
    m_currentShape = shape; // 保存当前形状

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

    m_currentActor = anActor; // 保存当前 Actor

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
    auto* viewer = static_cast<Viewer*>(clientData); // 强制转换为 Viewer
    
    std::string key = iren->GetKeySym();
    
    if (key == "c" || key == "C") {
        if (viewer->m_currentActor) {
            double r = (double)rand() / RAND_MAX;
            double g = (double)rand() / RAND_MAX;
            double b = (double)rand() / RAND_MAX;
            viewer->m_currentActor->GetProperty()->SetColor(r, g, b);
            iren->Render();
        }
    } else if ((key == "s" || key == "S") && iren->GetControlKey()) {
        if (!viewer->m_currentShape.IsNull()) {
            std::cout << "Saving shape to save.stp (Control + S)..." << std::endl;
            if (IO::SaveStep(viewer->m_currentShape, "save.stp")) {
                std::cout << "Successfully saved to save.stp" << std::endl;
            } else {
                std::cerr << "Failed to save to save.stp" << std::endl;
            }
        }
    }
}
