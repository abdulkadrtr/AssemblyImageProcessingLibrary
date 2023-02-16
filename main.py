import cv2
import functions as f

def main():
    photo_name = "plaka.jpg"
    img = cv2.imread(photo_name)
    cv2.imshow("image",img)
    cv2.waitKey(0)

    img = f.rgb2gray(photo_name)
    cv2.imshow("image",img)
    cv2.waitKey(0)
    f.sEdge(img)
    cv2.imshow("image",img)
    cv2.waitKey(0)

if __name__ == "__main__":
    main()
