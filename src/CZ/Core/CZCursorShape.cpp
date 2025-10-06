#include <CZ/Core/CZCursorShape.h>
#include <unordered_map>

const std::string &CZ::CZCursorShapeString(CZCursorShape shape) noexcept
{
    static const std::unordered_map<CZCursorShape, std::string> shapeToString
    {
        {CZCursorShape::Default, "Default"},
        {CZCursorShape::ContextMenu, "ContextMenu"},
        {CZCursorShape::Help, "Help"},
        {CZCursorShape::Pointer, "Pointer"},
        {CZCursorShape::Progress, "Progress"},
        {CZCursorShape::Wait, "Wait"},
        {CZCursorShape::Cell, "Cell"},
        {CZCursorShape::Crosshair, "Crosshair"},
        {CZCursorShape::Text, "Text"},
        {CZCursorShape::VerticalText, "VerticalText"},
        {CZCursorShape::Alias, "Alias"},
        {CZCursorShape::Copy, "Copy"},
        {CZCursorShape::Move, "Move"},
        {CZCursorShape::NoDrop, "NoDrop"},
        {CZCursorShape::NotAllowed, "NotAllowed"},
        {CZCursorShape::Grab, "Grab"},
        {CZCursorShape::Grabbing, "Grabbing"},
        {CZCursorShape::ResizeR, "ResizeR"},
        {CZCursorShape::ResizeT, "ResizeT"},
        {CZCursorShape::ResizeTR, "ResizeTR"},
        {CZCursorShape::ResizeTL, "ResizeTL"},
        {CZCursorShape::ResizeB, "ResizeB"},
        {CZCursorShape::ResizeBR, "ResizeBR"},
        {CZCursorShape::ResizeBL, "ResizeBL"},
        {CZCursorShape::ResizeL, "ResizeL"},
        {CZCursorShape::ResizeLR, "ResizeLR"},
        {CZCursorShape::ResizeTB, "ResizeTB"},
        {CZCursorShape::ResizeTRBL, "ResizeTRBL"},
        {CZCursorShape::ResizeTLBR, "ResizeTLBR"},
        {CZCursorShape::ResizeColumn, "ResizeColumn"},
        {CZCursorShape::ResizeRow, "ResizeRow"},
        {CZCursorShape::AllScroll, "AllScroll"},
        {CZCursorShape::ZoomIn, "ZoomIn"},
        {CZCursorShape::ZoomOut, "ZoomOut"},
        {CZCursorShape::DragAndDropAsk, "DragAndDropAsk"},
        {CZCursorShape::MoveOrResize, "MoveOrResize"},
    };

    auto it = shapeToString.find(shape);
    static const std::string unknown = "Unknown";
    return it != shapeToString.end() ? it->second : unknown;
}
