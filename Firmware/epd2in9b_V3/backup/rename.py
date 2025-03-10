import re

# 读取原始文件
input_file = "font8x8.h"
output_file = "font8x8_renamed.h"

with open(input_file, "r", encoding="utf-8") as f:
    lines = f.readlines()

# 处理文件内容
new_lines = []
hex_counter = 0  # 从 0x00 开始计数
for line in lines:
    # 正则匹配 (0xXX) 和后面的索引
    match = re.search(r"\(0x([0-9A-Fa-f]{2})\)\s+(\d+)", line)
    if match:
        new_hex = f"(0x{hex_counter:02X})"
        new_index = f"{hex_counter}"
        modified_line = re.sub(r"\(0x[0-9A-Fa-f]{2}\)", new_hex, line)
        modified_line = re.sub(r"\d+$", new_index, modified_line)
        hex_counter += 1
        new_lines.append(modified_line)
    else:
        new_lines.append(line)

# 保存修改后的文件
with open(output_file, "w", encoding="utf-8") as f:
    f.writelines(new_lines)

print(f"文件已处理并保存到: {output_file}")
