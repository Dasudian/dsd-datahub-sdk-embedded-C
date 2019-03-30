# Copy certificates for running the samples and tests provided with the SDK into this directory
# Certificates can be created and downloaded from the DSD IoT Console
# The IoT Client takes the full path of the certificates as an input parameter while initializing
# This is the default folder for the certificates only for samples and tests. A different path can be specified if required.
# The certificates files are just temporarily for testing, please do not directly use them for commercial use
# Follow the steps to generate device certs files, or ask Dasudian team for providing you the files

# 1. Create Device private key
openssl genrsa -out privkey.pem 2048
# 2. Create Device csr file
openssl req -new -out device.csr -key privkey.pem
# 3. Create the Device signed certificate, please ask Dasudian Team for the password
openssl x509 -req -in device.csr -CA rootCA.crt -CAkey ca.key -CAcreateserial -out cert.pem -days 5000

# You can alternatively ask Dasudian Team to generate the certificate files for you
# 企业客户请下载安装 [DSD App](https://dasudian.com/cn/dsd/dsd.html)，并注册成为企业用户，DSD IIoT平台将自动为客户生成专属的证书与加密文件
# 也可从苹果、华为等智能手机应用市场下载安装DSD工业智能App
