#pragma once

#include <TopoDS_Shape.hxx>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>

/**
 * @brief 简单的 OCCT 形状可视化查看器封装
 */
class Viewer {
public:
    Viewer();
    ~Viewer() = default;

    /**
     * @brief 添加一个 OCCT 形状到查看器中
     */
    void Show(const TopoDS_Shape& shape);

    /**
     * @brief 启动可视化窗口并进入事件循环
     */
    void Start();

private:
    /**
     * @brief 键盘事件回调函数
     */
    static void KeyPressCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);

    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;

    // 存储当前显示的形状和对应的 Actor，以便在回调中使用
    TopoDS_Shape m_currentShape;
    vtkSmartPointer<vtkActor> m_currentActor;
};
