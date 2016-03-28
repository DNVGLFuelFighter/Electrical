from __future__ import print_function
from PIL import Image
from PIL import ImageColor
import numpy as np
import sys

"""
    Outputs image in hex format for each pixel in c syntax and format for NeoPixel led matix.
    @arg: pixel_list 
    @todo:  write to file
            change name of const 
"""
def output_image(pixel_list ):
    out = "const int out_image[] = {\n"
    for idx, i in enumerate(pixel_list):
        out = out + i + ","
        if((idx+1) % 96 == 0):
            out = out + "\n"
       
    out = out[:-2] + "};"
    print(out)
#    print(len(out))
    return 0

def output_image_for_eth(pixel_list ):
    out = ""
    for idx, i in enumerate(pixel_list):
        out = out + str(int(i, 16)) + ","

    print(out)
#    print(len(out))
    return 0

"""
    Checks input argument, opens the image, reformats it to right format for NeoPixel matrix and call output_image function
    @todo   multiple images in arguments 
            other file formats not just gif
"""
if  __name__ == "__main__":
    im = Image.open(sys.argv[1])
#    print(im.format, im.size, im.mode)
    im.seek(0)
    try:
        while 1:
            im_tmp = im.convert('RGB') 
            pixel_np = np.array(im_tmp.getdata())
            pixel_hex = []
            for pix in pixel_np:
                color = "0x"
                for val in pix:
                    tmp = hex(val).replace('0x', "")
                    if(len(tmp) == 1):
                        tmp = '0'+tmp
                    color = color + tmp                
                pixel_hex.append(color)
        
            pixel_np2list = np.array(pixel_hex).reshape(8, len(pixel_hex)/8)
            pixel_lst = []
            for idx, column in enumerate(pixel_np2list.T):
                if(idx % 2 == 1):
                    #reverse column
                    pixel_lst.append(column[::-1].tolist())
                else:
                    #normal column
                    pixel_lst.append(column.tolist())
            pixel_lst = sum(pixel_lst, [])
#            print(pixel_lst)
            
            output_image_for_eth(pixel_lst) 
            im.seek(im.tell()+1)
    except EOFError:
        pass
    
