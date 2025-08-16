import os
import subprocess
import chardet
from xml.etree import ElementTree as ET

project_root = os.path.abspath(os.path.join(os.getcwd(), '..'))  # 当前目录的父目录
lessoncode_folder = os.path.join(project_root, 'LessonCode/')
example_folder = os.path.join(project_root, 'week06/homework/QtWord/')
mui_folder = os.path.join(project_root, 'mui')
bin_folder = os.path.join(mui_folder, 'bin')
zh_ts_folder = os.path.join(mui_folder, 'zh_CN')
ts_file = os.path.join(zh_ts_folder, 'translate_zh.ts')
os.makedirs(zh_ts_folder, exist_ok=True)
lupdate_tool = os.path.join(mui_folder, 'bin/lupdate.exe')

# 查找所有的.h和.cpp文件，忽略其他不相关的文件
def find_qt_project_files(directory):
    print(f"搜索当路径下：{directory}")
    qt_project_files = []
    valid_extensions = ('.h', '.cpp','.ui')

    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(valid_extensions):
                file_path = os.path.abspath(os.path.join(root, file))
                qt_project_files.append(file_path)

    return qt_project_files

def update_ts_file(qt_source_files):
    print(f"正在处理以下文件：{qt_source_files}")
    #生成一个临时文件，后面和原文件进行合并
    temp_ts_file = os.path.join(zh_ts_folder, 'temp_lessoncode.ts')
    print(f"lupdate的路径为:%1{lupdate_tool}")
    command = [lupdate_tool, '-no-obsolete',*qt_source_files, '-ts', temp_ts_file]

    try:
        subprocess.run(command, check=True)

        if os.path.exists(ts_file):
            with open(temp_ts_file, 'rb') as temp_file:
                raw_data = temp_file.read()
                result = chardet.detect(raw_data)
                temp_encoding = result['encoding']

            with open(ts_file, 'rb') as output_file:
                raw_data = output_file.read()
                result = chardet.detect(raw_data)
                ts_encoding = result['encoding']

            temp_tree = ET.parse(temp_ts_file, parser=ET.XMLParser(encoding=temp_encoding))
            temp_root = temp_tree.getroot()
            original_tree = ET.parse(ts_file, parser=ET.XMLParser(encoding=ts_encoding))
            original_root = original_tree.getroot()

            for context in temp_root.findall('context'):
                found = False
                for existing_context in original_root.findall('context'):
                    if existing_context.find('name').text == context.find('name').text:
                        # 合并 <message> 标签
                        for message in context.findall('message'):
                            # 检查是否已经存在相同的 <message> 标签
                            message_exists = False
                            for existing_message in existing_context.findall('message'):
                                if existing_message.find('source').text == message.find('source').text:
                                    message_exists = True
                                    break
                            if not message_exists:
                                existing_context.append(message)
                        found = True
                        break
                if not found:
                    original_root.append(context)
            # 保存合并后的文件
            with open(ts_file, 'wb') as output_file:
                output_file.write(ET.tostring(original_root, encoding='utf-8', method='xml'))
            os.remove(temp_ts_file)
        else:
            os.rename(temp_ts_file, ts_file)
            print(f"原TS文件不存在，生成新的TS文件：{ts_file}")

    except subprocess.CalledProcessError as e:
        print(f"错误: 生成TS文件时失败。错误信息：{e}")
def remove_line_numbers_from_ts(ts_file_path):
    try:
        tree = ET.parse(ts_file_path)
        root = tree.getroot()

        for message in root.findall('.//message'):
            # 查找并移除所有的 location 元素
            locations = message.findall('location')
            for location in locations:
                message.remove(location)

        # 保存修改后的 XML 文件
        tree.write(ts_file_path, encoding='utf-8', xml_declaration=True)
        print(f"已成功去除 {ts_file_path} 中的代码行数信息。")
    except Exception as e:
        print(f"处理文件 {ts_file_path} 时出错: {e}")

# 主程序
if __name__ == '__main__':
    qt_pinyin_files = find_qt_project_files(lessoncode_folder)
    update_ts_file(qt_pinyin_files)
    remove_line_numbers_from_ts(ts_file)