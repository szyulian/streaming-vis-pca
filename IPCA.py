from struct import unpack, calcsize, Struct, pack
import numpy as np
import psutil
from pathlib import Path
from scipy import sparse
from inc_pca import IncPCA

# gives a single float value
print(psutil.cpu_percent())
# gives an object with many fields
print(psutil.virtual_memory())

#struct_fmt = '=64H' # int[5], float, byte[255]
struct_fmt = '=64f';
struct_len = calcsize(struct_fmt)
struct_unpack = Struct(struct_fmt).unpack_from

print(struct_len)

filename = "/media/yulian/Data/rawfeaturesdata/gpusurffeatures.bin";
outfilename = "/media/yulian/Data/rawfeaturesdata/pcafeatures.bin";

ipca = IncPCA(20, 1.0);

start_idx = 0;
end_idx = 2;  #exclusive

for x in range(start_idx, end_idx):

    local_filename = filename + str(x);
    print("prepare open file: " + local_filename);
    filesize = Path(local_filename).stat().st_size;
    print("file size: " + str(filesize));
    print("elements: " + str(filesize // 4 // 64));
    results = np.zeros(shape=((filesize // 4 // 64),64), dtype=np.float64)
    idx = 0;
    with open(local_filename, "rb") as file:
        while True:
            data = file.read(struct_len)
            if not data: break;
            s = struct_unpack(data);
            results[idx] = s;
            idx = idx + 1;
            #if idx > 1000: break;
             
            #results[idx] = s;
            #idx = idx + 1;

    chunk_size = 100
    for i in range(0, idx//chunk_size):
        ipca.partial_fit(results[i*chunk_size : (i+1)*chunk_size]);

print(ipca.getMeans())
print(ipca.getComponents().transpose())

newFile = open("MeansNComponents.bin", "wb");
newFile.write(pack(struct_fmt, *ipca.getMeans()));
for i in range(0, 64):
    for j in range(0, 20):
        #print(ipca.getComponents().transpose()[j][i]);
        flt = np.float32(ipca.getComponents().transpose()[j][i]);
        flt.tofile(newFile);
        
newFile.close();

for x in range(start_idx, end_idx):

    local_filename = filename + str(x);
    print("prepare open file: " + local_filename);
    filesize = Path(local_filename).stat().st_size;
    print("file size: " + str(filesize));
    print("elements: " + str(filesize // 4 // 64));
    results = np.zeros(shape=((filesize // 4 // 64),64), dtype=np.float64)
    idx = 0;
    with open(local_filename, "rb") as file:
        while True:
            data = file.read(struct_len)
            if not data: break;
            s = struct_unpack(data);
            results[idx] = s;
            idx = idx + 1;
            #if idx > 1000: break;

    out_file = outfilename + str(x);
    outFile = open(out_file, "wb");
        
    for i in range(0, idx):
        out = ipca.transform(results[i : (i + 1)]);
        #print(out);
        s = pack('20f', *out[0]);
        outFile.write(s);
    outFile.close();


