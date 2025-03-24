import re

def hex_to_binary(hex_str):
    # 去掉大括号和逗号
    hex_str = hex_str.strip('{},')
    # 提取所有十六进制数
    hex_values = re.findall(r'0x[0-9A-Fa-f]+', hex_str)
    print("\n")
    print(hex_str)
    # 将每个十六进制数转换为二进制并打印
    for hex_value in hex_values:
        binary_value = bin(int(hex_value, 16))[2:].zfill(8)  # 转换为8位二进制
        print(binary_value)
    
    for hex_value in hex_values:
        binary_value = bin(int(hex_value, 16))[2:].zfill(8)  # 转换为8位二进制
        binary_display = binary_value.replace('0', ' ').replace('1', '*')
        print(binary_display)

def binary_to_hex(binary_lines):
    hex_values = []
    for binary_line in binary_lines:
        # 将二进制字符串转换为十六进制
        hex_value = hex(int(binary_line, 2))
        hex_values.append(hex_value)
    # 格式化输出
    hex_str = "{" + ", ".join(hex_values) + "}"
    print("\n转换后的十六进制格式：")
    print(hex_str)
    return hex_str

def multi_line_input():
    print("请输入多行二进制数据（每行8位），输入 'fin' 结束输入：")
    binary_lines = []
    while True:
        # 一次性读取多行输入
        lines = input().strip().split('\n')
        for line in lines:
            line = line.strip()
            if line.lower() == "fin":
                return binary_lines
            if len(line) != 8 or not all(c in '01' for c in line):
                print(f"错误：'{line}' 不是有效的8位二进制数据。")
                continue
            binary_lines.append(line)
        # 如果没有检测到 'fin'，继续等待输入
        print("继续输入更多二进制数据，或输入 'fin' 结束：")

if __name__ == "__main__":
    while True:
        user_input = input("请输入数组（格式如 {0x02, 0x42, 0xFE, ...}），或输入 'b' 进入多行二进制输入模式，或输入 q 退出：")
        if user_input.lower() == "q":
            print("程序已退出。")
            break
        elif user_input.lower() == "b":
            binary_lines = multi_line_input()
            if binary_lines:
                hex_str = binary_to_hex(binary_lines)
                hex_to_binary(hex_str)
        else:
            hex_to_binary(user_input)