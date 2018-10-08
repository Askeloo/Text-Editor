#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

class NumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void numberAreaPaintEvent(QPaintEvent *event);
    int numberAreaWidth();
    void setBackgroundColor(QColor color);


protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void statsCounted(int chars, int words);

private slots:
    void updateNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateNumberArea(const QRect &, int);
    void countStats();

private:
    QWidget *number_area;
};


class NumberArea : public QWidget
{
public:
    NumberArea(CodeEditor *editor) : QWidget(editor)
    {
        code_editor = editor;
    }

    //recommended size for widget
    QSize sizeHint() const override
    {
        return QSize(code_editor->numberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        code_editor->numberAreaPaintEvent(event);
    }

private:
    CodeEditor *code_editor;
};

#endif
