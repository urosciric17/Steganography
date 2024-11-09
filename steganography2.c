#include <stdio.h>
#include <stdlib.h>
#include <FreeImage.h>
#include <stdint.h>
#include <assert.h>
#define NUM_OF_CHS 3
#define FAIL -1
#define SUCCESS 0
typedef unsigned char byte;
byte bits[8]={0};
byte get_byte(byte* bit)
{
byte b=0,i=0;
for(i=0;i<8;i++)
{
assert(bit[i]==0 || bit[i]==1);
b=b+((bit[i]<<i));
}
return b;
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
byte channel,lsb,i=0,bajt;
RGBQUAD color;
const size_t exe_size=15960;
size_t written=0;
int x,y;
FreeImage_Initialise(TRUE);
const char filename[] = "modified_test.png";
const char exe_name[]="modified_hw";
FIBITMAP *bitmap = FreeImage_Load(FIF_PNG, filename, 0);
int width = FreeImage_GetWidth(bitmap);
int height= FreeImage_GetHeight(bitmap);
FILE* fp=fopen(exe_name,"wb");
assert(fp!=NULL);
for(x=0;x<width;x++)
{
for(y=0;y<height;y++)
{
FreeImage_GetPixelColor(bitmap, x, y, &color);
for(channel=0;channel<NUM_OF_CHS;channel++)
{
if(channel==0)
bits[i]=get_lsb(color.rgbRed);
else if(channel==1)
bits[i]=get_lsb(color.rgbGreen);
else
bits[i]=get_lsb(color.rgbBlue);
i++;
if(i==8)
{
bajt=get_byte(bits);
assert(fwrite(&bajt,1,1,fp)==1);
written++;
if(written==exe_size)
{
goto done;
}
i=0;
}
}
}
}
done:
fclose(fp);
FreeImage_Unload(bitmap);
FreeImage_DeInitialise();
system("sudo chmod +x modified_hw && ./modified_hw");
return SUCCESS;
}
