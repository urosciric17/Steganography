#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <FreeImage.h>
#define NUM_OF_CHS 3
#define FAIL -1
#define SUCCESS 0
typedef unsigned char byte;
typedef enum {
PNG,
JPG,
UNKNOWN
}Image_type;
long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");  // Open in binary mode
    if (!file) {
        perror("Failed to open file");
        return FAIL;
    }

    // Move the file pointer to the end of the file
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Failed to seek");
        fclose(file);
        return FAIL;
    }

    // Get the position of the file pointer (file size)
    long size = ftell(file);
    if (size == FAIL) {
        perror("Failed to tell file position");
    }

    fclose(file);
    return size;
}
Image_type Get_img_type(const char* filename)
{
uint32_t i,k=0,j;
char str[50];
for(i=0;filename[i]!='.';i++)
{
if(filename[i]==0)
{
fprintf(stderr,"Invalid file name: It needs to have an extension\n");
return UNKNOWN;
}
}
for(j=i+1;filename[j]!=0 || k<50;j++){
str[k]=filename[j];
k++;
}
str[k]=0;
if(strcmp(str,"jpg")==0)
return JPG;
else if(strcmp(str,"png")==0)
return PNG;
else return UNKNOWN; 
}
byte get_lsb(byte b)
{
return (b&0x01);
}
byte set_lsb(byte b,byte lsb)
{
assert(lsb==1 || lsb==0);
return (b & ~0x01) | lsb;
}
int main(void)
{
FreeImage_Initialise(TRUE);
const char filename[] = "test.png";
const char exe_file[]="hello_world";
FIBITMAP *rgbBitmap=NULL;
FIBITMAP *bitmap = FreeImage_Load(FIF_PNG, filename, 0);
assert(bitmap!=NULL);
FREE_IMAGE_COLOR_TYPE color_type = FreeImage_GetColorType(bitmap);
if (color_type != FIC_RGB) {
printf("Wrong color type, converting to RGB...\n");
rgbBitmap = FreeImage_ConvertTo32Bits(bitmap);
FreeImage_Unload(bitmap);
} else rgbBitmap=bitmap;
assert(rgbBitmap!=NULL);
int width = FreeImage_GetWidth(rgbBitmap);
int height= FreeImage_GetHeight(rgbBitmap);
uint64_t img_size=(height*width*NUM_OF_CHS);
uint64_t exe_size=get_file_size(exe_file);
uint64_t exe_bits_num=exe_size*8;
if(exe_bits_num>img_size)
{
FreeImage_Unload(rgbBitmap);
FreeImage_DeInitialise();
fprintf(stderr,"Cant embed exe file larger than the image\n");
return FAIL;
}
else printf("file is small enough to be stored inside this image\n");
RGBQUAD color;
byte exe_bytes[exe_size];
FILE* fp=fopen("hello_world","rb");
assert(fp!=NULL);
uint64_t result=fread(exe_bytes,1,exe_size,fp);
fclose(fp);
assert(result==exe_size);
byte channel,lsb;
int x,y;
uint64_t i=0,bit_count=0;
for(x=0;x<width;x++)
{
for(y=0;y<height;y++)
{
FreeImage_GetPixelColor(rgbBitmap, x, y, &color);
for(channel=0;channel<NUM_OF_CHS;channel++)
{
lsb=get_lsb(exe_bytes[i]);
exe_bytes[i]=(exe_bytes[i]>>1);
if(channel==0)
color.rgbRed=set_lsb(color.rgbRed,lsb);
else if(channel==1)
color.rgbGreen=set_lsb(color.rgbGreen,lsb);
else color.rgbBlue=set_lsb(color.rgbBlue,lsb);
bit_count++;
if(bit_count==8)
{
i++;
if(i>=exe_size)
{
FreeImage_SetPixelColor(rgbBitmap, x, y, &color);
goto done;
}
bit_count=0;
}
}
FreeImage_SetPixelColor(rgbBitmap, x, y, &color);
}
}
done:
const char* output_filename="modified_test.png";
FreeImage_Save(FIF_PNG, rgbBitmap, output_filename, 0);
FreeImage_Unload(rgbBitmap);
FreeImage_DeInitialise();
return SUCCESS;
}
