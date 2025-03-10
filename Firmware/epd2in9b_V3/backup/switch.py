import re

def process_font_data(input_file, output_file):
    # 读取原始数据
    with open(input_file, 'r', encoding='utf-8') as f:
        content = f.read()

    # 增强正则表达式：匹配字符数据行
    pattern = re.compile(
        r'''
        \{                     # 起始花括号
        \s*([0-9A-Fx,\s]+)\s*  # 十六进制数据部分
        \}\s*,\s*//\s*         # 结束花括号和注释标记
        (.*?)\s*               # 注释内容
        (?=\{|$)               # 前瞻断言：下一个字符是{或文件结尾
        ''', 
        re.VERBOSE
    )

    matches = pattern.findall(content)
    
    new_lines = []
    for data_part, comment_part in matches:
        try:
            # 处理十六进制数据
            hex_values = []
            for x in data_part.split(','):
                x_clean = x.strip().lower().replace('0x', '')
                if not x_clean:
                    continue
                hex_values.append(int(x_clean, 16))
            
            if len(hex_values) != 8:
                print(f"警告：数据长度错误 -> {data_part}")
                continue
                
            # 检查是否需要处理（排除A/B/D/E）
            char_match = re.search(r'\b([ABDE])\b', comment_part)
            if not char_match:  # 需要处理的情况
                # 执行行交换操作
                swapped = hex_values[::-1]  # 直接反转列表实现行交换
            else:              # 不需要处理的情况
                swapped = hex_values.copy()
            
            # 生成新行
            hex_str = ', '.join([f'0x{b:02X}' for b in swapped])
            new_line = f"    {{{hex_str}}}, // {comment_part.strip()}"
            new_lines.append(new_line)
            
        except Exception as e:
            print(f"处理失败：{data_part} -> {str(e)}")

    # 写入新文件
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("#ifndef FONT8X8_FR_MODIFIED_H\n")
        f.write("#define FONT8X8_FR_MODIFIED_H\n\n")
        f.write("#include <avr/pgmspace.h>\n\n")
        f.write("const uint8_t Font8x8_FR[224][8] PROGMEM = {\n")
        f.write('\n'.join(new_lines))
        f.write("\n};\n\n#endif\n")

# 使用示例
process_font_data('font8x8.h', 'font8x8_modified.h')