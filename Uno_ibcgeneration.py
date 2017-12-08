#!/usr/bin/python3

import sys
import re

def help():
 print ( """
Help for this code generator.
This generator generates code in a correctly tagged code file in c++ style.

Arguments : [Rule] [Output]

    Rule    Is a file which specifies an IBC ruleset in form of numbers in a pattern [MID] [REQSIZE] [RESSIZE]
    Output  Is a file where the output will go. It will be searched for a tags which look like this.
            
                /* IBC_FRAME_GENERATION_TAG_BEGIN */

                /* IBC_FRAME_GENERATION_TAG_END */
            
            this script will not touch any lines outside these tags. This script will terminate when no tags are found in [Output]
            This script will also preserve custom code inside of tags

                /* IBC BEGIN [MID] [RECV/SEND]

                /* IBC END [MID] [RECV/SEND]
 """)

helptags = ["-h", "--help"]

if len(sys.argv) < 2 or sys.argv[1] in helptags :
 help()
 exit()

#scan for rules

rulefile = open(sys.argv[1], 'r')
rules = rulefile.readlines()
rulefile.close()

p = re.compile('^\s*\d{1,3}\s*\d{1,3}\s*\d{1,3}')

rules = [x.strip('\n').split(' ') for x in rules if p.match(x)]
rules = [[int(x) for x in l] for l in rules]

print("Rules found :")
print(rules)

#scan for code tags in output
ofile = open(sys.argv[2] , 'r')
olines = ofile.readlines()
ofile.close()


begintagline = 0 
endtagline = 0

begintag = 'IBC_FRAME_GENERATION_TAG_BEGIN'
endtag = 'IBC_FRAME_GENERATION_TAG_END'

i = 0
while i < len(olines):
 if begintag in olines[i]:
  begintagline = i
 if endtag in olines[i]:
  endtagline = i
 i = i+1

if begintagline > endtagline:
 printf('Error parsing Outfile !')

olines = olines[begintagline : endtagline+1]

#find to preserve code parts

pid = re.compile('\d{1,3}')

taglines = []

i=0
while i< len(olines):
 if 'IBC_PRESERVE_RECV_BEGIN' in olines[i] or 'IBC_PRESERVE_RECV_END' in olines[i] or 'IBC_PRESERVE_SEND_BEGIN' in olines[i] or 'IBC_PRESERVE_SEND_END' in olines[i]:
  taglines.append(i)
   
 i = i+1

#check length of tagslist found

if len(taglines)%4 != 0:
 print('Error in parsing preservation. exiting')
 exit()

#MID:( RECVBEGIN, RECVEND, SENDBEGIN, SENDEND)

tagmap = {}
#prepare tagmap
for e in rules:
 tagmap[e[0]] = [0,0,0,0]

for linenr in taglines:
 l = olines[linenr]
 m = re.search('\d{1,3}', olines[linenr])
 if 'BEGIN' in l:
  if 'SEND' in l:
   tagmap[int(m.group(0))][2] = linenr
  elif 'RECV' in l:
   tagmap[int(m.group(0))][0] = linenr
 elif 'END' in l:
  if 'SEND' in l:
   tagmap[int(m.group(0))][3] = linenr
  elif 'RECV' in l:
   tagmap[int(m.group(0))][1] = linenr
 
print(tagmap)

sendpreserve = {}
recvpreserve = {}

for e in tagmap:
 if tagmap[e][:2] == [0,0]:
  s = [""]
 else :
  s = olines[tagmap[e][0]-1:tagmap[e][1]+1]
 recvpreserve[e] = s

 if tagmap[e][2:] == [0,0]:
  s = [""]
 else :
  s = olines[tagmap[e][2]-1:tagmap[e][3]]
 sendpreserve[e] = s
 

for e in recvpreserve:
 print (e, ":")
 print (recvpreserve[e])


for e in sendpreserve:
 print (e, ":")
 print (sendpreserve[e])

def dummy_recv(num):
 ret =  ['char buff['+str(num)+'];\n','ibc.recv(buff,'+str(num)+');\n','\n','//DONT FORGET TO HASH\n','ibc.setDH(ibc.createDH(buff,'+str(num)+');\n']
 #indent
 ret = ['\t\t\t'+x for x in ret]
 return ret


def dummy_send(num):
 ret =  ['for(int i = 0 ; i<'+str(num)+';i++) {ibc.send(0)}\n','\n','//DONT FORGET TO HASH\n','ibc.setDH(0);\n']
 ret = ['\t\t\t'+x for x in ret]
 return ret

def dummy_recv_dyn():
 num = 'ibc.inSIZE_DYN()'
 ret =  ['char *buff = new char ['+str(num)+'];\n','ibc.recv(buff,'+str(num)+');\n','\n','//DONT FORGET TO HASH\n','ibc.setDH(ibc.createDH(buff,'+str(num)+');\n','delete[] buff;\n']
 #indent
 ret = ['\t\t\t'+x for x in ret]
 return ret


def dummy_send_dyn():
 ret =  ['for(int i = 0 ; i< ?DYNAMIC_SIZE? ;i++) {ibc.send(0)}\n','\n','//DONT FORGET TO HASH\n','ibc.setDH(0);\n']
 ret = ['\t\t\t'+x for x in ret]
 return ret

print(rules)

for e in recvpreserve:
 if recvpreserve[e] == [""]:
  rule = [x for x in rules if x[0] == e]
  if rule[0][1] == 255:
   recvpreserve[e] = dummy_recv_dyn()
  else:
   recvpreserve[e] = dummy_recv(rule[0][1])

for e in sendpreserve:
 if sendpreserve[e] == [""]:
  rule = [x for x in rules if x[0] == e]
  if rule[0][2] == 255:
   sendpreserve[e] = dummy_send_dyn()
  else:
   sendpreserve[e] = dummy_send(rule[0][2])

for e in recvpreserve:
 print (e, ":")
 print (recvpreserve[e])

for e in sendpreserve:
 print (e, ":")
 print (sendpreserve[e])
