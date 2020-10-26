# Affine Cipher encryptor/decryptor character device

This character device driver can be used to encrypt and decrypt messages using the Affine Cipher. 



## Compilation

  In order to compile the code, open a terminal inside the code directory and run the following command (we will be using $(uname -r) to avoid kernel version errors) 
  
    sudo make -C /lib/modules/$(uname -r)/build M=$PWD modules
  
  The Makefile will take care of the compilation from there.
  
  
  
 
## Insert Module

  Once the module is compiled, certain files will be created. We will inserting the .ko file into the list of modules using the following command
    
    sudo insmod ./enc_module9.ko
   
  Run the following command to make sure the module is loaded
  
    lsmod | grep enc
  
  
  
## Making a node to take in the input and output
 
  The C program will need a file to get the buffers from. Hence we will make a node with the following command:
  
    sudo mknod -m 666 /dev/my_device c 239 0
    
  Here, 666 indicates read-write permissions and the c indicates it is a character device driver. 239 is the major number associated with the device.
  
  
  

## Compile and run the C program

    gcc userprgm.c && ./a.out <0|1> #Select 0 for encryption, 1 for decryption
    
    
    
## Debugging

  In order to know what is happening, we have also sent printk() commands which can be read using the following command:
    
    dmesg
    
  
