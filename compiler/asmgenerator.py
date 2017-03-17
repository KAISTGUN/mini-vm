a= input("number : ")
opcode= raw_input("opcode : ")
N1= input("operand1 : ")
N2= input("operand2 : ")

for i in range(a):  	
   	print opcode+" "+"r"+str(N1)+","+"r"+str(N2)
   	N1 = N1+1      
  	N2 = N2+1
