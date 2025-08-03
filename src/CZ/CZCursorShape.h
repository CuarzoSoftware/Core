#ifndef CZ_CURSORSHAPE_H
#define CZ_CURSORSHAPE_H

#include <CZ/Cuarzo.h>

namespace CZ
{
    /**
     * @brief Common cursor shapes
     *
     * @see https://wayland.app/protocols/cursor-shape-v1#wp_cursor_shape_device_v1:enum:shape
     */
    enum class CZCursorShape : UInt32
    {
        /// The standard default arrow cursor.
        Default = 1,

        /// Indicates a context menu is available for the object under the cursor.
        ContextMenu = 2,

        /// Indicates help is available for the object under the cursor.
        Help = 3,

        /// Indicates a link or other interactive element (typically a hand/pointer).
        Pointer = 4,

        /// Indicates an operation is in progress; user should wait.
        Progress = 5,

        /// Indicates the program is busy; user input is deferred until completion.
        Wait = 6,

        /// Indicates a cell or collection of cells can be selected.
        Cell = 7,

        /// Simple crosshair, often used for precision pointing.
        Crosshair = 8,

        /// Indicates text can be selected or edited.
        Text = 9,

        /// Indicates vertical text can be selected or edited.
        VerticalText = 10,

        /// Drag-and-drop: an alias (shortcut) will be created if dropped.
        Alias = 11,

        /// Drag-and-drop: the dragged item will be copied if dropped.
        Copy = 12,

        /// Drag-and-drop: the dragged item will be moved if dropped.
        Move = 13,

        /// Drag-and-drop: dropping is not permitted at the current location.
        NoDrop = 14,

        /// Drag-and-drop: the requested action is disallowed.
        NotAllowed = 15,

        /// Drag-and-drop: something can be grabbed for dragging.
        Grab = 16,

        /// Drag-and-drop: something is currently being grabbed.
        Grabbing = 17,

        /// Resizing: the right edge is being adjusted.
        ResizeR = 18,

        /// Resizing: the top edge is being adjusted.
        ResizeT = 19,

        /// Resizing: the top-right corner is being adjusted.
        ResizeTR = 20,

        /// Resizing: the top-left corner is being adjusted.
        ResizeTL = 21,

        /// Resizing: the bottom edge is being adjusted.
        ResizeB = 22,

        /// Resizing: the bottom-right corner is being adjusted.
        ResizeBR = 23,

        /// Resizing: the bottom-left corner is being adjusted.
        ResizeBL = 24,

        /// Resizing: the left edge is being adjusted.
        ResizeL = 25,

        /// Resizing: both left and right edges are being adjusted (horizontal resize).
        ResizeLR = 26,

        /// Resizing: both top and bottom edges are being adjusted (vertical resize).
        ResizeTB = 27,

        /// Resizing: top-right and bottom-left corners are being adjusted.
        ResizeTRBL = 28,

        /// Resizing: top-left and bottom-right corners are being adjusted.
        ResizeTLBR = 29,

        /// Resizing: a column (horizontal dimension) can be resized.
        ResizeColumn = 30,

        /// Resizing: a row (vertical dimension) can be resized.
        ResizeRow = 31,

        /// Indicates content can be scrolled in any direction.
        AllScroll = 32,

        /// Indicates zoom-in is available.
        ZoomIn = 33,

        /// Indicates zoom-out is available.
        ZoomOut = 34,

        /// Drag-and-drop: user will be prompted to choose which action to perform. (non-CSS value)
        DragAndDropAsk = 35,

        /// Resizing: the item can be moved or resized in any direction. (non-CSS value)
        MoveOrResize = 36,
    };
}

#endif // CZ_CURSORSHAPE_H
