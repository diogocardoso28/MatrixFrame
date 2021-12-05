from PIL import Image
from numpy import asarray

image = Image.open("Images/hearth.png")

print("Original Image: \n")
print(image.format)
print(image.mode)
print(image.size)

resizedImage = image.resize((16, 16))
resizedImage = resizedImage.convert('RGBA')
imgWidth, imgHeight = resizedImage.size
data = asarray(resizedImage)
print(type(data))
# summarize shape
print(data.shape)

# print("Resized Image: \n")
# print(resizedImage.format)
# print(resizedImage.mode)
# print(resizedImage.size)
# create Pillow image
image2 = Image.fromarray(data)
print(type(image2))
image2.show()
# summarize image details
print(image2.mode)
print(image2.size)
# create Pillow image
image2 = Image.fromarray(data)
print(type(image2))

# summarize image details
print(image2.mode)
print(image2.size)
