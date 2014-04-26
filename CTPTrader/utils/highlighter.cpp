#include <QtGui>
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::red);
    keywordFormat.setFontWeight(QFont::Bold);
}

void Highlighter::highlightBlock(const QString &text)
{
    qDebug() << "highlightBlock------- call";
    foreach(const HighlightingRule &rule,highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index,length,rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}

void Highlighter::setTextQueue(const QStringList& textQueue)
{
    highlightingRules.clear();
    HighlightingRule rule;

    const QString tmp("\\b");
    foreach(const QString& str,textQueue)
    {
        QString pattern(tmp);
        pattern += str;
        pattern += tmp;
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}
