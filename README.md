# AssemblyImageProcessingLibrary
 Image processing library created by writing Inline Assembly
 
 
![Assembly](https://user-images.githubusercontent.com/87595266/216832356-e73aa040-4f59-406b-a87c-7afabb2896b1.png)

# Proje Hakkında
Bu projede, aşağıda yer alan tüm görüntü işleme algoritmaları Inline Assembly kullanılarak yazılmıştır.

`import functions as f` yazarak projenize dahil edebilirsiniz. Proje dosyanız 32bit olarak derlenmelidir.

- Sobel Edge Detection

    Bu fonksiyonu kullanmak için grayscale dönüşüm yaptığınız fotoğraf değişkenini kullanarak
    
    `f.sEdge(img)`  olarak çağırın.
- Gaussian Blur

    Bu fonksiyonu kullanmak için grayscale dönüşüm yaptığınız fotoğraf değişkenini kullanarak
    
    `f.gBlur(img)`  olarak çağırın.
    
- Rgb2Gray
    
    Bu fonksiyonu kullanmak için görsel yolunu girin. 
    
    `img = f.rgb2gray("lena.png")`
- Erosion

    Bu fonksiyonu kullanmak için grayscale dönüşüm yaptığınız fotoğraf değişkenini kullanarak
    
    `f.erosion(img,kernel_size)` olarak çağırın. kernel_size değişkeni tek sayı olmalıdır.
- Dilation

    Bu fonksiyonu kullanmak için grayscale dönüşüm yaptığınız fotoğraf değişkenini kullanarak
    
    `f.dilation(img,kernel_size)` olarak çağırın. kernel_size değişkeni tek sayı olmalıdır.

# Youtube Proje Tanıtım Videosu
https://youtu.be/bCF_3d_csyk

## AssemblyProject Dosyası Hk.

Bu dosya yalnızca erosion, dilation morfolojik işlemlerini kapsayan ayrı bir projedir.

Proje içerisindeki `20011042.pdf` dokümanında AssemblyProject proje kodlarının ayrıntılı açıklaması yer almaktadır.

AssemblyImageProcessingLibrary ile bir ilişiği yoktur.

