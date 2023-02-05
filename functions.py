import cv2
import ctypes

func = ctypes.WinDLL("imgProcessing/imgProcessing.dll")
func.rgb2gray.argtypes = [ctypes.POINTER(ctypes.c_ubyte),ctypes.POINTER(ctypes.c_ubyte),ctypes.POINTER(ctypes.c_ubyte),ctypes.c_int]
func.erosion.argtypes = [ctypes.POINTER(ctypes.c_ubyte),ctypes.c_int,ctypes.c_int,ctypes.c_int]
func.dilation.argtypes = [ctypes.POINTER(ctypes.c_ubyte),ctypes.c_int,ctypes.c_int,ctypes.c_int]
func.gBlur.argtypes = [ctypes.POINTER(ctypes.c_ubyte),ctypes.c_int,ctypes.c_int]
func.sEdge.argtypes = [ctypes.POINTER(ctypes.c_ubyte),ctypes.c_int,ctypes.c_int]

def rgb2gray(file_name):
    img = cv2.imread(file_name)
    b,g,r = cv2.split(img)
    r_ptr = r.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
    g_ptr = g.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
    b_ptr = b.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
    func.rgb2gray(r_ptr, g_ptr, b_ptr,r.shape[0]*r.shape[1])
    return r

def erosion(img,mask_size):
    if(mask_size%2==0):
        print("mask size must be odd")
        return
    func.erosion(img.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),mask_size,img.shape[0],img.shape[1]) 

def dilation(img,mask_size):
    if(mask_size%2==0):
        print("mask size must be odd")
        return
    func.dilation(img.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),mask_size,img.shape[0],img.shape[1])

def gBlur(img):
    func.gBlur(img.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),img.shape[0],img.shape[1])

def sEdge(img):
    func.sEdge(img.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),img.shape[0],img.shape[1])