#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\blw\\b" << "\\blb\\b" << "\\blbu\\b" << "\\blh\\b" << "\\blhu\\b"
                    << "\\bsw\\b" << "\\bsb\\b" << "\\bsh\\b"
                    << "\\badd\\b" << "\\baddu\\b" << "\\bsub\\b" << "\\bsubu\\b"
                    << "\\bslt\\b" << "\\bsltu\\b"
                    << "\\band\\b" << "\\bor\\b" << "\\bxor\\b" << "\\bnor\\b"
                    << "\\bsll\\b" << "\\bsrl\\b" << "\\bsra\\b"
                    << "\\baddi\\b" << "\\baddiu\\b" << "\\bandi\\b" << "\\bori\\b" << "\\bxori\\b"
                    << "\\blui\\b" << "\\bslti\\b" << "\\bsltiu\\b"
                    << "\\bbeq\\b" << "\\bbne\\b" << "\\bblez\\b" << "\\bbgtz\\b" << "\\bbltz\\b" << "\\bbgez\\b"
                    << "\\bj\\b" << "\\bjal\\b" << "\\bjalr\\b" << "\\bjr\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    registerFormat.setFontWeight(QFont::Bold);
    registerFormat.setForeground(Qt::darkMagenta);
    QStringList registerPatterns;
    registerPatterns << "\\$[t][0-8]" << "\\$[s][0-7]" << "\\$[a][0-3]" << "\\$[v][0-1]"
                     << "\\$zero" << "\\$gp" << "\\$sp" << "\\$fp" << "\\$ra" << "\\$at";
    foreach (const QString &pattern, registerPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = registerFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat_sharp.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat_sharp;
    highlightingRules.append(rule);


    singleLineCommentFormat_slash.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = singleLineCommentFormat_slash;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setFontWeight(QFont::Bold);
    functionFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+:");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
