#pragma once

#include <TopoDS_Shape.hxx>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkTextActor.h>
#include <vector>
#include <string>

#include "log/MSG.hpp"

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

    /**
     * @brief 清空当前显示的形状
     */
    void Clear();

    /**
     * @brief 回退到最初显示的那个形状
     */
    void ResetShape();

    /**
     * @brief 在界面上显示一条消息
     * @param msg 消息内容
     * @param level 日志级别 (Info: 绿色, Warning: 黄色, Error: 红色)
     */
    void Log(const std::string& msg, LogLevel level = LogLevel::Info);

private:
    /**
     * @brief 键盘事件回调函数
     */
    static void KeyPressCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData);

    /**
     * @brief 更新文本显示位置和内容
     */
    void UpdateLogDisplay();

    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;

    // 存储当前显示的形状和对应的 Actor，以便在回调中使用
    TopoDS_Shape m_currentShape;
    vtkSmartPointer<vtkActor> m_currentActor;

    // 存储初始显示的形状
    TopoDS_Shape m_initialShape;

    // 消息日志 Actor 列表 (每行一个 Actor 以支持不同颜色)
    std::vector<vtkSmartPointer<vtkTextActor>> m_logActors;
    const size_t m_maxLogLines = 10;
    const int m_fontSize = 20;
    const int m_lineSpacing = 25;
};
