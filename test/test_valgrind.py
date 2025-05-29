import re
import argparse
import os

def check_memory_freed(valgrind_output_file):
    leak = 0

    with open(valgrind_output_file, 'r') as file:
        content = file.read()
    
    heap_summary_section = re.search(r'==\d+== HEAP SUMMARY:', content)
    if heap_summary_section:
        summary_index = heap_summary_section.start()
        summary_content = content[summary_index:]

        if re.search(r'==\d+== All heap blocks were freed -- no leaks are possible', summary_content):
            leak = 0
        else:
            leak = 1


    return leak

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('valgrind_file', type=str)
    parser.add_argument('part_marks', type=int)

    with open("test/temporary.txt", "r") as file:
        lines = file.readlines()
        leak = check_memory_freed(parser.parse_args().valgrind_file)
        marks = int(lines[0].strip())
        total= int(lines[1].strip())
    
        part_marks = 0
        if not leak:
            part_marks += parser.parse_args().part_marks
            
        print(f"\033[35mTesting memory leaks:\033[0m {part_marks}/{parser.parse_args().part_marks}\n")
        print(f"\033[1m\033[35mTotal:\033[0m {marks+part_marks}/{total}\n")


    os.remove("test/temporary.txt")