
ta = 0.12092882991556092
t_cf = 2.493765586034913 
T = 2.6146944159504737
Vm = 15
v_max =10 


for i in range(0, int(2.62 / 0.1) + 1):
    time = i * 0.1
    if ( time < ta ):
        print("Accelerating")
        
    elif ( Vm <= v_max ):
        print("Decelerating, no constant speed")
    elif( (time >= ta) and (time <= t_cf) ):
        print("Constant speed region")
    else:
        print("Decelerating")
