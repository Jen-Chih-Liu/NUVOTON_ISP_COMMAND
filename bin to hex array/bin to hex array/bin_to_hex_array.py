my_file = "test.bin"
blob_data = bytearray()
with open(my_file, 'rb') as file_t:
    blob_data = bytearray(file_t.read())
count=0
print("array size=%d" %len(blob_data))
with open("a.txt", "w") as f:
    f.write("array size=%d \n\r" %len(blob_data))
    f.write("array[%d]= \n" %len(blob_data))
    f.write("{ \n")
    for te in blob_data:
        temp=hex(te)
        temp=temp+", "
        f.write(temp)
        count=count+1
        if (count==16):
            f.write("\n")
            count=0
    f.write("}; \n\r")

