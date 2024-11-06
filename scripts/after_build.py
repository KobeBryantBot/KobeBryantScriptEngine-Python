import json
import os
import shutil


# 查找第一个.dll文件
def find_first_dll(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".dll"):
                return os.path.join(root, file)
    return None


# 读取manifest.json文件
def read_manifest(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        return json.load(file)


# 写入到manifest.json文件
def write_to_manifest(data, folder_path):
    # 如果文件夹已存在，先删除
    if os.path.exists(folder_path):
        shutil.rmtree(folder_path)
    os.makedirs(folder_path, exist_ok=True)
    file_path = os.path.join(folder_path, "manifest.json")

    with open(file_path, "w", encoding="utf-8") as file:
        json.dump(data, file, indent=4)


# 替换{PluginName}为dllname
def replace_plugin_name(data, dllname):
    if isinstance(data, dict):
        for key, value in data.items():
            if isinstance(value, str) and "{PluginName}" in value:
                data[key] = value.replace("{PluginName}", dllname)
            else:
                replace_plugin_name(value, dllname)
    elif isinstance(data, list):
        for item in data:
            replace_plugin_name(item, dllname)


# 主函数
def main():
    manifest_path = "../manifest.json"
    bin_directory = "../bin/DLL"
    dll_file_path = find_first_dll(bin_directory)

    if not dll_file_path:
        print("No .dll file found in the specified directory.")
        return

    # 获取dllname
    dllname = os.path.splitext(os.path.basename(dll_file_path))[0]

    folder_name = os.path.join("../bin", dllname)

    # 检查manifest.json是否存在
    if not os.path.exists(manifest_path):
        print(f"Error: {manifest_path} does not exist.")
        return

    # 读取manifest.json
    manifest_data = read_manifest(manifest_path)

    # 替换{PluginName}
    replace_plugin_name(manifest_data, dllname)

    # 写入manifest.json到dllname文件夹
    write_to_manifest(manifest_data, folder_name)

    # 复制.dll文件到dllname文件夹
    shutil.copy(dll_file_path, folder_name)


if __name__ == "__main__":
    main()
