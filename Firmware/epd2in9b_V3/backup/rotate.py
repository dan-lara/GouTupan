import re

def process_font_data(input_file, output_file):
    # 读取原始数据
    with open(input_file, 'r', encoding='utf-8') as f:
        content = f.read()

    # 增强正则表达式：允许注释包含括号和特殊字符
    pattern = re.compile(
        r'''
        \{                     # 起始花括号
        \s*([0-9A-Fx,\s]+)\s*  # 十六进制数据部分(含空格)
        \}\s*,\s*//\s*         # 结束花括号和注释标记
        (.*?)\s*               # 注释内容(非贪婪匹配)
        (?=\{|$)               # 前瞻断言：下一个字符是{或文件结尾
        ''', 
        re.VERBOSE
    )

    # 查找所有匹配项
    matches = pattern.findall(content)
    
    # 处理每个匹配项
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
            
            # 验证数据长度
            if len(hex_values) != 8:
                print(f"警告：数据长度错误 -> {data_part}")
                continue
                
            # 执行行列转换（核心逻辑）
            converted_bytes = row_to_column_conversion(hex_values)
            
            # 生成新行
            hex_str = ', '.join([f'0x{b:02X}' for b in converted_bytes])
            new_line = f"    {{{hex_str}}}, // {comment_part.strip()}"
            new_lines.append(new_line)
            
        except ValueError as e:
            print(f"数据转换失败：{data_part} -> {e}")

    # 写入新文件
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("#ifndef FONT8X8_FR_TRANSFORMED_H\n")
        f.write("#define FONT8X8_FR_TRANSFORMED_H\n\n")
        f.write("#include <avr/pgmspace.h>\n\n")
        f.write("const uint8_t Font8x8_FR[224][8] PROGMEM = {\n")
        f.write('\n'.join(new_lines))
        f.write("\n};\n\n#endif\n")

def row_to_column_conversion(original_bytes):
    """严格的行列转换函数（行转列）"""
    # 构建8x8二进制矩阵（高位在前）
    matrix = []
    for byte in original_bytes:
        matrix.append([(byte >> (7 - i)) & 1 for i in range(8)])
    
    # 执行行列转置
    converted = []
    for col in range(8):  # 新字节数=8
        new_byte = 0
        for row in range(8):  # 原行号=新列号
            # 将每个位移动到正确位置（高位在前）
            new_byte = (new_byte << 1) | matrix[row][col]
        converted.append(new_byte)
    
    return converted

# 使用示例
process_font_data('font8x8.h', 'font8x8_transposed.h')