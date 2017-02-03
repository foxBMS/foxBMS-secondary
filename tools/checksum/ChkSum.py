#!/usr/bin/python
# -*- coding: UTF-8 -*-
'''
@copyright &copy; 2010 - 2016, Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V. All rights reserved.

BSD 3-Clause License
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1.  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3.  Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

We kindly request you to use one or more of the following phrases to refer to foxBMS in your hardware, software, documentation or advertising materials:

&Prime;This product uses parts of foxBMS&reg;&Prime;

&Prime;This product includes parts of foxBMS&reg;&Prime;

&Prime;This product is derived from foxBMS&reg;&Prime;
'''

'''
@author: FoxBMS team
'''

'''
This script will be invoked in a post-build step to generate either a STM32 compatible CRC32 checksum 
or a simple Modulo32BitAddition of the provided binary.
For this purpose the generated .hex file will be loaded from disc and its content is being parsed.
In the next step a buffer file will be created, initialized with 0xFF to match the pattern during flash
and parsed content from the hex file is stored into this filebuffer. Each line gets its own line-chksum
and desired chksum type is calculated and written into a headerfile in the final step.
'''

# import needed libraries
import sys
import os
import ConfigParser
from bitstring import Bits
import binascii
import numpy as np
import struct

# global defines
VERSION =                   ("V5.Z")

# define global variables
pBufferFilePtrDstAdd = 0x00000000
DstPosFinalChksumErr = False

SrcLineDataLengthCpy = 0
SrcLineAddressCpy = []
SrcLineRecordTypeCpy = 0
SrcLineDataCpy = []

# ini file class, used to store needed information about the input binary
class IniFile:
    def __init__(self, ProjectType, HexFilePath, ChecksumHeaderFilePath, FlashHeaderAdress, BeginAddress, EndAddress, ChksumAddress, ChecksumAlgo, CrcPolynomial):
        self.ProjectType = ProjectType
        self.HexFilePath = HexFilePath
        self.ChecksumHeaderFilePath = ChecksumHeaderFilePath
        self.FlashHeaderAdress = FlashHeaderAdress
        self.BeginAddress = BeginAddress
        self.EndAddress = EndAddress
        self.ChksumAddress = ChksumAddress
        self.ChecksumAlgo = ChecksumAlgo
        self.CrcPolynomial = CrcPolynomial

# helper function to convert from hex to U8 
def HexasciiToU8(LineString, Position):
    a = Bits(hex=LineString[Position:Position + 2]) 
    return np.uint8(a.int)

# helper function to convert from hex to U16 
def HexasciiToU16(LineString, Position):
    a = Bits(hex=LineString[Position:Position + 4]) 
    return np.uint16(a.int)

# helper function to convert from hex to U32 
def HexasciiToU32(LineString, Position):
    a = Bits(hex=LineString[Position:Position + 8]) 
    return np.uint32(a.int)

# helper function to swap endianess of input parama 
def endian_swap(x):
    return (((x << 24) & 0xFF000000) | ((x <<  8) & 0x00FF0000) | ((x >>  8) & 0x0000FF00) | ((x >> 24) & 0x000000FF))

# print out common information about this tool
def PrintInfo():
    print("************************************************************************\n") 
    print("*** Post-Build Program ChkSum.exe, " + str(VERSION) + "\n\n") 
    print("*** In the first build the elf-file will contain a wrong checksum value, \n") 
    print("*** only *.hex-file will be modified with a valid checksum !!!\n\n") 
    print("*** A definition file (e.g. checksum.h) will be generated\n") 
    print("*** which provides the right definition of macro BUILD_TOOL_CHECKSUM.\n\n") 
    print("*** By a secondary build process this macro (with the right checksum)\n") 
    print("*** will be integrated in the source code.\n") 
    print("*** Then also the elf-file will contain the right checksum. \n") 
    print("************************************************************************\n") 
    print("Calculating Flash Checksum...\n") 

# function to parse alle needed informations from the config file, using Python ConfigParser module
def ParseConfig(FilePath, ConfigStruct):
    Config = ConfigParser.ConfigParser() 
    
    try:
        Config.read(FilePath) 
            
        ConfigStruct.ProjectType = Config.get('project', 'projecttype') 
        
        ConfigStruct.HexFilePath = Config.get('file', 'path') 
        
        ConfigStruct.ChecksumHeaderFilePath = Config.get('file', 'checksumheader')
         
        ConfigStruct.FlashHeaderAdress = Config.get('addresses', 'flashheader')
        a = long(Cfg.FlashHeaderAdress, 16)
        a &= 0x00ffffff
        ConfigStruct.FlashHeaderAdress = a
        
        ConfigStruct.BeginAddress = Config.get('addresses', 'beginaddress')
        a = long(ConfigStruct.BeginAddress, 16)
        a &= 0x00ffffff
        ConfigStruct.BeginAddress = a
         
        ConfigStruct.EndAddress = Config.get('addresses', 'endaddress') 
        a = long(ConfigStruct.EndAddress, 16)
        a &= 0x00ffffff
        ConfigStruct.EndAddress = a
        
        ConfigStruct.ChksumAddress = Config.get('addresses', 'chksumaddress')
        a = long(ConfigStruct.ChksumAddress, 16)
        a &= 0x00ffffff
        ConfigStruct.ChksumAddress = a 
        
        ConfigStruct.ChecksumAlgo = Config.get('checksum', 'checksumalgorithm') 
        
        ConfigStruct.CrcPolynomial = Config.get('crc-polynomial', 'polynomial') 
        
        return True  
    except:
        print("Error! Ini-File not found in path \n" + os.getcwd()) 
        return False 
    
# entrypoint of our application, everything starts here
if __name__ == '__main__':
    # print out common informations
    PrintInfo() 
    
    #/---- Check for arguments -----
    if len(sys.argv) < 2:   #There have to be 2 arguments: file.exe path-to-ini-file
        print("**************************** E R R O R *********************************\n") 
        print("Error! Check your arguments. \nFor help use: " + str(sys.argv[0]) + " -help") 
    elif sys.argv[1] == "-help":     #If second argument is "help", printout the help
       
        print("\nUsage: %s ini-file\n\nRelative Path: ./path/file.ini\nAbsolute Path: Part:/path/file.ini\n" % sys.argv[0]) 
  
    #---- Section to read from the chksum.ini file ------------------------------------------------
    else:
        IniLocation = sys.argv[1] 
        SrcFileCurrLine = 0 
        
        # initialize config with some default values
        Cfg = IniFile('', '', '', 0x00000000, 0x00000000, 0x00000000, 0x00000000, '', 0x00000000) 
         
        if ParseConfig(IniLocation, Cfg):
            
            with open(Cfg.HexFilePath, "rb+") as pSrcFile, open("binbuffer_ohne_Chksum.bin", "wb+") as pBufferFile:    #open source hex-file, path from the ini-file
                
                for i in range(0x200000):    #Fill bin-file with 0x1fffff 0xFF
                    pBufferFile.write("%c" % 0xFF) 
                
                pBufferFile.seek(0) # go to beginning of the file for futher operations
                FinalDataChksum = 0x00000000
                  
                while True: # iterate whole file content and parse it line by line
                    SrcFileCurrLine += 1 
                    SrcFileLineBuffer = pSrcFile.readline() 
                    if not SrcFileLineBuffer: break # EOF
                   
                    SrcLineAddress = []
                    SrcLineDataLength = HexasciiToU8(SrcFileLineBuffer, 1)      # Convert the SrcLineData length
                    SrcLineAddress.append(HexasciiToU8(SrcFileLineBuffer, 3))   # Convert the offset address
                    SrcLineAddress.append(HexasciiToU8(SrcFileLineBuffer, 5))   # Convert the offset address
                    SrcLineRecordType = HexasciiToU8(SrcFileLineBuffer, 7)      # Convert the record type
                    
                    SrcLineChksumNewCalc = 0 
                    SrcLineData = [] 
                    
                    for i in range(SrcLineDataLength):
                        SrcLineData.append(HexasciiToU8(SrcFileLineBuffer, i*2+9))  # Convert the data
                        SrcLineChksumNewCalc += SrcLineData[i]                      # pre calc for checksum
                    
                    SrcLineChksum = HexasciiToU8(SrcFileLineBuffer, SrcLineDataLength*2+9)  # Convert line checksum
                    
                    SrcLineChksumNewCalc += (int(SrcLineDataLength) + int(SrcLineAddress[0]) + int(SrcLineAddress[1]) + int(SrcLineRecordType) + int(SrcLineChksum)) # calculated value must be 0
                   
                    if np.uint8(SrcLineChksumNewCalc) != 0x00: # sum of all bytes inclusive the checksum must be 0x00, print error when != 0
                        print("**************************** E R R O R *********************************\n") 
                        print("Error! Checksum of line %d not correct." % SrcFileCurrLine)
                    else:
                        if SrcLineRecordType == 0x04:                               # changing the two higher byte of SrcLineAddress if record-typ is 0x04 
                            pBufferFilePtrDstAdd = 0x00000000
                            pBufferFilePtrDstAdd = np.uint32(SrcLineData[1] << 16)  # shifting 2 bytes
                            
                        pBufferFilePtrDstAdd &= 0xFFFF0000                          # mask the lower 2 bytes
                        pBufferFilePtrDstAdd |= np.uint8(SrcLineAddress[0]) << 8    # add the lower byte
                        pBufferFilePtrDstAdd |= np.uint8(SrcLineAddress[1])         # add the lowest byte
                        
                        if SrcLineRecordType == 0x00:                               # if record-typ declared as Data (record type == 0x00), then write into buffer file at the position (from SrcLineAddress)
                            pBufferFile.seek(pBufferFilePtrDstAdd) 
                            for i in range(np.int32(SrcLineDataLength)):
                                pBufferFile.write("%c" % np.uint8(SrcLineData[i]))  # write SrcLineData into binary buffer file
                        
                        # pre-calc for destination of final checksum if data record type and adress within checksum position
                        if pBufferFilePtrDstAdd <= Cfg.ChksumAddress and (pBufferFilePtrDstAdd + int(SrcLineDataLength) - 4) >= Cfg.ChksumAddress and (SrcLineRecordType == 0x00):
                            DstPosFinalChksum = pSrcFile.tell() - int(SrcLineDataLength) * 2 - 4  
                            DstPosNewLineChksum =  pSrcFile.tell() - 4
                            # define DstPosFinalChksum as start of the SrcLineData bytes
                            # tell => position at end of line
                            # - SrcLineDataLength*2 => minus the SrcLineDataLength in ascii
                            # -4 => minus line-SrcLineChksum and 0xA and 0xD (ascii: newline)
                            DstPosFinalChksum = DstPosFinalChksum + (Cfg.ChksumAddress - pBufferFilePtrDstAdd) * 2  # calc the destination of final SrcLineChksum position
                            DstPosFinalChksumErr = True # position for final data-checksum is found, no error
                
                            # copy the destination line of the final data checksum (the line checksum has to recalculate!)
                            SrcLineDataLengthCpy = SrcLineDataLength;
                            SrcLineAddressCpy.append(SrcLineAddress[0])
                            SrcLineAddressCpy.append(SrcLineAddress[1])
                            SrcLineRecordTypeCpy = SrcLineRecordType
                            for i in range(np.int32(SrcLineDataLength)):
                                SrcLineDataCpy.append(SrcLineData[i])
                
                        if Cfg.FlashHeaderAdress == pBufferFilePtrDstAdd:
                            FlashHeaderHexStart = pSrcFile.tell() - 4 - np.int32(SrcLineDataLength) * 2


                if DstPosFinalChksumErr == False:
                    print("**************************** E R R O R *********************************\n")
                    print("Error! Position for final checksum not found.")
                else:
                    pBufferFile.seek(Cfg.BeginAddress)                                                  # get the start address for the calculating SrcLineChksum
                    SrcFile4BytBuff = struct.unpack('I', pBufferFile.read(4))[0]                        # read 4 bytes
                    ChksumStartAddress = 0x08000000 & 0x00ffffff#SrcFile4BytBuff & 0x00ffffff           # convert to long and mask the highest byte
                       
                    pBufferFile.seek(Cfg.EndAddress)                                                    # get the end address for the calculating SrcLineChksum
                    SrcFile4BytBuff = struct.unpack('I', pBufferFile.read(4))[0]                        # read 4 bytes
                    ChksumEndAddress = (0x080FFF00-4) & 0x00ffffff#SrcFile4BytBuff & 0x00ffffff         # convert to long and mask the highest byte
                    pBufferFile.seek(ChksumStartAddress)
                    
                    ChksumCalcLength = 131072#ChksumEndAddress - ChksumStartAddress
                    
                    # calculate Modulo-32-Bit-Addition
                    if Cfg.ChecksumAlgo == 'modulo32bitaddition':
                        ChksumCalcLength /= 4                                                           # calc the count for 4 byte steps
                        while ChksumCalcLength > 0:
                            SrcFile4BytBuff = struct.unpack('I', pBufferFile.read(4))[0]                # read 4 bytes
                            FinalDataChksum += SrcFile4BytBuff                                          # calculate the addition-checksum
                            FinalDataChksum &= 0xFFFFFFFF                                               # mask 32bit to prevent python from using auto-types
                            ChksumCalcLength -= 1
                               
                    if Cfg.ChecksumAlgo == 'crc32':                                                     # calc crc32, using python binascii module, use mask to ensure compatible output                         
                        FinalDataChksum = (binascii.crc32(pBufferFile.read(ChksumCalcLength), 0) & 0xFFFFFFFF)
                         
                    FinalDataChksumWritten = endian_swap(FinalDataChksum)                               # swap endianess
                    pSrcFile.seek(DstPosFinalChksum)
                    pSrcFile.write("%08X" % FinalDataChksumWritten)                                     # write chcksum
                    
                    SrcLineAddressCpyTemp = 0
                    SrcLineAddressCpyTemp &= 0x0000FFFF
                    SrcLineAddressCpyTemp |= np.uint8(SrcLineAddressCpy[0]) << 8
                    SrcLineAddressCpyTemp |= np.uint8(SrcLineAddressCpy[1])
                    
                    # re-calc new line checksum for changed line and overwrite the new data checksum 
                    idx = (Cfg.ChksumAddress & 0x0000ffff) - np.uint16(SrcLineAddressCpyTemp)
                    SrcLineDataCpy[0 + idx] = np.int8(FinalDataChksum & 0x000000ff)
                    SrcLineDataCpy[1 + idx] = np.int8((FinalDataChksum & 0x0000ff00) >> 8)
                    SrcLineDataCpy[2 + idx] = np.int8((FinalDataChksum & 0x00ff0000) >> 16)
                    SrcLineDataCpy[3 + idx] = np.int8((FinalDataChksum & 0xff000000) >> 24)
                      
                    for i in range(np.int32(SrcLineDataLengthCpy)):
                        SrcLineChksumNewCalc -= SrcLineDataCpy[i]
                    
                    #  calc new line chcksum     
                    SrcLineChksumNewCalc -= (int(SrcLineDataLengthCpy) + int(SrcLineAddressCpy[0]) + int(SrcLineAddressCpy[1]) + int(SrcLineRecordTypeCpy))
                 
                    pSrcFile.seek(DstPosNewLineChksum)
                    pSrcFile.write("%02X" % np.uint8(SrcLineChksumNewCalc))                         # write new line chcksum
                
                    print("32-bit SW-Chksum:    0x%08X (hex-written: 0x%08X)\n" % (FinalDataChksum, FinalDataChksumWritten))
                    print("************************************************************************\n")    
                    
                    
                    pBufferFile.seek(Cfg.FlashHeaderAdress + 24)
            
            
            
            
            # create checkum header file and write "#define ...."
            with open(Cfg.ChecksumHeaderFilePath, "w+") as pBufferFilePtr:                
                pBufferFilePtr.write("/**\n")
                pBufferFilePtr.write(" *\n");
                pBufferFilePtr.write(" * This file is generated by checksum tool which is called by Post Build Process\n")
                pBufferFilePtr.write(" * in a batch file ! \n")
                pBufferFilePtr.write(" * After having generated this file the batch file will start a rebuild.\n")
                pBufferFilePtr.write(" * This means in the first Post Build Process a new Build will be started with the the right checksum.\n")
                pBufferFilePtr.write(" *\n")
                pBufferFilePtr.write("*/\n\n")
                pBufferFilePtr.write(" #define BUILD_TOOL_CHECKSUM       0x%08X" % FinalDataChksum)
                #pBufferFilePtr.write("Name %d [%-10.10s]\n",n,name);