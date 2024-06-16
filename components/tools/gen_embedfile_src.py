# gen_embedfile_src.py
import sys
import os

def gen_embedfile_src(input_file, output_file):
    print(input_file)
    print(output_file)
    
    with open(input_file, 'rb') as f:
        data = f.read()

    array_name = '_binary_' + os.path.basename(input_file).replace('.', '_').replace('/', '_')
    with open(output_file, 'w') as f:
        f.write('#include <stdint.h>\r\n\r\n')
        f.write(f'uint8_t {array_name}[] = {{')
        
        for i, byte in enumerate(data):
            if i % 12 == 0:
                f.write('\n    ')
            f.write(f'0x{byte:02x}, ')
            
        f.write('\n};\n')
        f.write(f'uint32_t {array_name}_len = {len(data)};\n')



if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: gen_embedfile_src.py <input_file> <output_file>")
        sys.exit(1)
        
    data_file = sys.argv[1]
    output_file = sys.argv[2]
    
    gen_embedfile_src(data_file, output_file)

