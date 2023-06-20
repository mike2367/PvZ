import os


class Static(object):
    def __init__(self, option):
        self.record_file = open("dir_statics.txt", "w+")# 保存统计结果
        self.option = option

    # 获取目录下文件名并记录每个文件行数
    def dir_statics(self, file_dir):
        total_count = 0
        for root, dirs, files in os.walk(file_dir):  # 路径， 文件夹， 文件
            for file in files:
                # 忽略此文件本身
                if file == "file_static.py":
                    continue
                file_type = file.split('.')
                if len(file_type) > 1:
                    if file_type[1] not in ["c", "cpp", "h", "hpp", "py"]:
                        continue
                else:
                    continue

                file_name = root + "\\" + file
                note_count, line_count = self.count_lines(file_name, file_type[1])
                total_count += line_count
                total_count += note_count
                print(file + "\tcode line:" + '\t' + repr(line_count), file=self.record_file)  # 将统计结果可观的打印到txt中
                print(file + "\tnote line:" + '\t' + repr(note_count), file=self.record_file)
        print('total count :\t' + repr(total_count), file=self.record_file)

    # 统计文件有效代码行数
    def count_cpp_notes(self, fp):
        line_count = 0
        note_count = 0
        in_note = False
        for line in fp.readlines():
            if not line.split():  # 判断是否为空行
                line.strip()  # 去除空行
                continue
            line = line.strip()
            if line.startswith("/"):
                if line[1] == "/":
                    note_count += 1
                    continue
                elif line[1] == "*":
                    in_note = True
                    note_count += 1
                    continue
                else:
                    note_count += 1
                    continue
            elif line.endswith("/"):
                in_note = False
                note_count += 1
                continue
            else:
                if in_note:
                    note_count += 1
                else:
                    line_count += 1
        return note_count, line_count

    def count_python_notes(self, fp):
        line_count = 0
        note_count = 0
        in_note = False
        for line in fp.readlines():
            if not line.split():  # 判断是否为空行
                line.strip()  # 去除空行
                continue
            line = line.strip()
            if line.startswith("#"):
                note_count += 1
                continue
            elif line.startswith("'"):
                in_note = True
                note_count += 1
                continue
            elif line.endswith("'"):
                in_note = False
                note_count += 1
                continue
            else:
                if in_note:
                    note_count += 1
                else:
                    line_count += 1
        return note_count, line_count

    def count_lines(self, file_name, file_type):
        fp = open(file_name, 'r+', encoding='utf-8')
        line_count = 0
        if self.option == 1:
            for line in fp.readlines():
                if not line.split():  # 判断是否为空行-
                    line.strip()  # 去除空行
                    continue
                else:
                    line_count += 1

            fp.close()
            return line_count

        else:
            if (file_type == "cpp") | (file_type == "c") | (file_type == "h") | (file_type == "hpp"):
                note_count, line_count = self.count_cpp_notes(fp)
            else:
                note_count, line_count = self.count_python_notes(fp)

        return note_count, line_count

    def run(self):
        cur_path = os.path.split(os.path.realpath(__file__))[0]  # 获取脚本所在绝对路径，分割完成后返回路径
        self.dir_statics(cur_path)
        self.record_file.close()


if __name__ == "__main__":
    option = input("input static type: with notes:1, without notes:2")
    stat = Static(option)
    stat.run()
    input("Press any key to exit...")

