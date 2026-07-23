#include "Text.h"

const char* ccontrol::Text::GetText() const
{
    return text;
}

void ccontrol::Text::SetText(const char* str)
{
    text = str;
    strLenOfChar = CalcStrWidth(str);
    strLen = strlen(str);
}

void ccontrol::Text::DrawSelf(HANDLE handle)
{
    short tx = x + (width - strLenOfChar) / 2;
    short ty = y + (height - 1) / 2;
    DWORD written;
    WriteConsoleOutputCharacter(handle, text, strLen, {tx, ty}, &written);
}

int Utf8SeqLen(unsigned char c) {
    if (c < 0x80) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1; // 无效的 UTF-8 序列，按单字节处理
}

uint32_t DecodeUtf8(const std::string& s, size_t& i) {
    if (i >= s.size()) return 0xFFFD; // 替换字符

    uint32_t cp = static_cast<unsigned char>(s[i++]);
    
    if (cp < 0x80) return cp;
    
    int seqLen = Utf8SeqLen(static_cast<unsigned char>(cp));
    
    if (i + seqLen - 1 > s.size()) {
        return 0xFFFD; // 序列不完整，返回替换字符
    }
    
    if ((cp & 0xE0) == 0xC0) {
        cp = (cp & 0x1F) << 6 | (static_cast<unsigned char>(s[i++]) & 0x3F);
    } else if ((cp & 0xF0) == 0xE0) {
        uint8_t b2 = static_cast<unsigned char>(s[i++]);
        uint8_t b3 = static_cast<unsigned char>(s[i++]);
        cp = (cp & 0x0F) << 12 | (b2 & 0x3F) << 6 | (b3 & 0x3F);
    } else if ((cp & 0xF8) == 0xF0) {
        uint8_t b2 = static_cast<unsigned char>(s[i++]);
        uint8_t b3 = static_cast<unsigned char>(s[i++]);
        uint8_t b4 = static_cast<unsigned char>(s[i++]);
        cp = (cp & 0x07) << 18 | (b2 & 0x3F) << 12 | (b3 & 0x3F) << 6 | (b4 & 0x3F);
    } else {
        return 0xFFFD; // 无效的 UTF-8 起始字节
    }
    
    return cp;
}

// 判断字符是否为宽字符（基于 Unicode East Asian Width 属性）
// 宽字符包括：Wide (W), Fullwidth (F), Ambiguous (A) 在东亚环境中
bool IsWideChar(uint32_t cp) {
    // CJK 统一汉字及扩展
    if (cp >= 0x4E00 && cp <= 0x9FFF) return true;  // CJK Unified Ideographs
    if (cp >= 0x3400 && cp <= 0x4DBF) return true;  // CJK Unified Ideographs Extension A
    if (cp >= 0x20000 && cp <= 0x2FFFD) return true; // CJK Unified Ideographs Extension B-I
    if (cp >= 0x30000 && cp <= 0x3FFFD) return true; // CJK Unified Ideographs Extension G-J
    
    // CJK 兼容汉字
    if (cp >= 0xF900 && cp <= 0xFAFF) return true;  // CJK Compatibility Ideographs
    if (cp >= 0x2F800 && cp <= 0x2FA1F) return true; // CJK Compatibility Ideographs Supplement
    
    // Hangul
    if (cp >= 0x1100 && cp <= 0x11FF) return true;  // Hangul Jamo
    if (cp >= 0xAC00 && cp <= 0xD7AF) return true;  // Hangul Syllables
    if (cp >= 0xD7B0 && cp <= 0xD7FF) return true;  // Hangul Jamo Extended-B
    
    // CJK 符号和标点
    if (cp >= 0x3000 && cp <= 0x303F) return true;  // CJK Symbols and Punctuation
    if (cp >= 0xFF00 && cp <= 0xFFEF) return true;  // Halfwidth and Fullwidth Forms
    
    // CJK 部首和笔画
    if (cp >= 0x2E80 && cp <= 0x2EFF) return true;  // CJK Radicals Supplement
    if (cp >= 0x2F00 && cp <= 0x2FDF) return true;  // Kangxi Radicals
    if (cp >= 0x31C0 && cp <= 0x31EF) return true;  // CJK Strokes
    if (cp >= 0x2FF0 && cp <= 0x2FFF) return true;  // Ideographic Description Characters
    
    // CJK 兼容形式
    if (cp >= 0xFE10 && cp <= 0xFE1F) return true;  // Vertical Forms
    if (cp >= 0xFE30 && cp <= 0xFE4F) return true;  // CJK Compatibility Forms
    
    // 表情符号（Emoji Presentation）
    if (cp >= 0x1F600 && cp <= 0x1F64F) return true; // Emoticons
    if (cp >= 0x1F300 && cp <= 0x1F5FF) return true; // Misc Symbols and Pictographs
    if (cp >= 0x1F680 && cp <= 0x1F6FF) return true; // Transport and Map
    if (cp >= 0x1F1E0 && cp <= 0x1F1FF) return true; // Flags (Regional Indicator Symbols)
    if (cp >= 0x2600 && cp <= 0x26FF) return true;   // Misc Symbols
    if (cp >= 0x2700 && cp <= 0x27BF) return true;   // Dingbats
    if (cp >= 0x1F900 && cp <= 0x1F9FF) return true; // Supplemental Symbols and Pictographs
    if (cp >= 0x1FA00 && cp <= 0x1FA6F) return true; // Chess Symbols
    if (cp >= 0x1FA70 && cp <= 0x1FAFF) return true; // Symbols and Pictographs Extended-A
    
    // 其他东亚符号
    if (cp >= 0x3200 && cp <= 0x32FF) return true;  // Enclosed CJK Letters and Months
    if (cp >= 0x3300 && cp <= 0x33FF) return true;  // CJK Compatibility
    if (cp >= 0xF0000 && cp <= 0xFFFFD) return true; // Supplementary Private Use Area-A
    if (cp >= 0x100000 && cp <= 0x10FFFD) return true; // Supplementary Private Use Area-B
    
    return false;
}

short ccontrol::Text::CalcStrWidth(const std::string& s) {
    short width = 0;

    for (size_t i = 0; i < s.size();) {
        uint32_t cp = DecodeUtf8(s, i);
        width += IsWideChar(cp) ? 2 : 1;
    }

    return width;
}
