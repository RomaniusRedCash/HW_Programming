V = [0,2,6,13, 21,32,48,67,99,134,198
    ];

n = 0:length(V)-1;
xlim([0,length(V)-1]);
ylim([0, max(V)]);

xlabel('Количество пар дисков');
ylabel('Ходы');

grid on;
grid minor;

hold on
plot (n,V,'Color','magenta','LineStyle','none','MarkerSize',15,'Marker','.','DisplayName','Данные');

plot(n,6*2.^(n./2)+3/2*n-9, "Color",'red','LineStyle','-.','DisplayName','Чётные');

plot(n,8*2.^((n-1)./2)+3*((n-1)./2)-6, "Color",'blue','LineStyle','-.','DisplayName','Нечётные');

legend('Location','southeast');