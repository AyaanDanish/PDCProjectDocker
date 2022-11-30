
red = "#FF0000"; green = "#00FF00"; blue = "#0000FF"; skyblue = "#87CEEB";
set style data histogram 
set style histogram cluster
set style fill solid
set boxwidth 0.9
set title "Time Comparison"
plot "data.txt" using 1:xtic(1) title "Parallel" linecolor rgb red,   \
     "data.txt" using 2:xtic(2) title "Serial" linecolor rgb blue
     