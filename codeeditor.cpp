#include <QtWidgets>

#include "codeeditor.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{

    number_area = new NumberArea(this);
    setBackgroundColor(QColor(0,0,40,10));

    connect(this, SIGNAL(textChanged()), this, SLOT(countStats()));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));


    updateNumberAreaWidth(0);
    highlightCurrentLine();
}


int CodeEditor::numberAreaWidth()
{
    int digits = 1;
    int max = blockCount();
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 5;
    space += fontMetrics().width(QLatin1Char('0')) * digits;
    return space;
}

void CodeEditor::setBackgroundColor(QColor color)
{
    QPalette p = this->palette();
    p.setColor(QPalette::Active, QPalette::Base, color);
    this->setPalette(p);
}


void CodeEditor::updateNumberAreaWidth(int)
{
    setViewportMargins(numberAreaWidth(), 0, 0, 0);
}


void CodeEditor::updateNumberArea(const QRect &rect, int dy)
{
    if (dy)
        number_area->scroll(0, dy);
    else
        number_area->update(0, rect.y(), number_area->width(), rect.height());

}

void CodeEditor::countStats()
{
    unsigned chars_count = toPlainText().length();
    unsigned words_count = toPlainText().split(QRegExp("\\W+"), QString::SkipEmptyParts).count();

    emit statsCounted(chars_count, words_count);
}


void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    number_area->setGeometry(QRect(cr.left(), cr.top(), numberAreaWidth(), cr.height()));
}


void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(0,0,200,40);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);

}


void CodeEditor::numberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(number_area);
    painter.fillRect(event->rect(), QColor(0,0,40,40));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();  //y
    int bottom = top + (int) blockBoundingRect(block).height();                      //y+h


    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(0,0,40,150));
            painter.drawText(0, top, number_area->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }

}
