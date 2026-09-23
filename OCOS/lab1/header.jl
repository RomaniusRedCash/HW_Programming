T_s(f_s)=1/f_s
L(M)=2^M # 3
dU(U_min,U_max,M)=(U_max - U_min)/2^M # 4

x_D(n,f_s,U_min,U_max,M,x_A)=floor((x_A(n*T_s(f_s)))/(U_max - U_min)*2^M) # 1
x_D_z(n,f_s,U_min,U_max,M,x_A)=min(max(x_D(n,f_s,U_min,U_max,M,x_A),0),2^M-1) # 1*

x(n,f_s,U_min,U_max,M,x_A)=x_D(n,f_s,U_min,U_max,M,x_A)*(U_max - U_min)/2^M # 2
x_D_z2(n,f_s,U_min,U_max,M,x_A)=min(max(x(n,f_s,U_min,U_max,M,x_A),0),U_max-dU(U_min,U_max,M)) # 2*

F=[[1000,-5,5,16],[10,0,3,3]]
