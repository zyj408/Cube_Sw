function out = pushfifo(in,num)
    persistent ad_fifo1;
    persistent ad_fifo2;
    persistent ad_fifo3;
    persistent ad_fifo4;
    persistent ad_fifo5;
    persistent ad_fifo6;
    persistent ad_fifo7;
    persistent ad_fifo8;
    persistent ad_fifo9;
    persistent ad_fifo10;
    persistent ad_fifo11;
    persistent ad_fifo12;
    persistent ad_fifo13;
    persistent ad_fifo14;
    persistent ad_fifo15;
    persistent ad_fifo16;
    %persistent temp_buf;
    %persistent vol_buf;
    
%%

    if isempty(ad_fifo1)
        ad_fifo1 = zeros([1,200]);
    end
    if isempty(ad_fifo2)
        ad_fifo2 = zeros([1,200]);
    end
    if isempty(ad_fifo3)
        ad_fifo3 = zeros([1,200]);
    end
    if isempty(ad_fifo4)
        ad_fifo4 = zeros([1,200]);
    end
    if isempty(ad_fifo5)
        ad_fifo5 = zeros([1,200]);
    end
    if isempty(ad_fifo6)
        ad_fifo6 = zeros([1,200]);
    end
    if isempty(ad_fifo7)
        ad_fifo7 = zeros([1,200]);
    end
    if isempty(ad_fifo8)
        ad_fifo8 = zeros([1,200]);
    end
    if isempty(ad_fifo9)
        ad_fifo9 = zeros([1,200]);
    end
    if isempty(ad_fifo10)
        ad_fifo10 = zeros([1,200]);
    end
    if isempty(ad_fifo11)
        ad_fifo11 = zeros([1,200]);
    end
    if isempty(ad_fifo12)
        ad_fifo12 = zeros([1,200]);
    end
    if isempty(ad_fifo13)
        ad_fifo13 = zeros([1,200]);
    end
    if isempty(ad_fifo14)
        ad_fifo14 = zeros([1,200]);
    end
    if isempty(ad_fifo15)
        ad_fifo15 = zeros([1,200]);
    end
    if isempty(ad_fifo16)
        ad_fifo16 = zeros([1,200]);
    end
%%
    if num == 1
    for i = 1 : (200 - 1)
        ad_fifo1(i) = ad_fifo1(i+1);
    end
    ad_fifo1(200) = in;
    out = ad_fifo1;
    end
    
    if num == 2
    for i = 1 : (200 - 1)
        ad_fifo2(i) = ad_fifo2(i+1);
    end
    ad_fifo2(200) = in;
    out = ad_fifo2;
    end
    
    if num == 3
    for i = 1 : (200 - 1)
        ad_fifo3(i) = ad_fifo3(i+1);
    end
    ad_fifo3(200) = in;
    out = ad_fifo3;
    end
    
    if num == 4
    for i = 1 : (200 - 1)
        ad_fifo4(i) = ad_fifo4(i+1);
    end
    ad_fifo4(200) = in;
    out = ad_fifo4;
    end
    
    if num == 5
    for i = 1 : (200 - 1)
        ad_fifo5(i) = ad_fifo5(i+1);
    end
    ad_fifo5(200) = in;
    out = ad_fifo5;
    end
    
    if num == 6
    for i = 1 : (200 - 1)
        ad_fifo6(i) = ad_fifo6(i+1);
    end
    ad_fifo6(200) = in;
    out = ad_fifo6;
    end
    
    if num == 7
    for i = 1 : (200 - 1)
        ad_fifo7(i) = ad_fifo7(i+1);
    end
    ad_fifo7(200) = in;
    out = ad_fifo7;
    end
    
    if num == 8
    for i = 1 : (200 - 1)
        ad_fifo8(i) = ad_fifo8(i+1);
    end
    ad_fifo8(200) = in;
    out = ad_fifo8;
    end
    
    if num == 9
    for i = 1 : (200 - 1)
        ad_fifo9(i) = ad_fifo9(i+1);
    end
    ad_fifo9(200) = in;
    out = ad_fifo9;
    end
    
    if num == 10
    for i = 1 : (200 - 1)
        ad_fifo10(i) = ad_fifo10(i+1);
    end
    ad_fifo10(200) = in;
    out = ad_fifo10;
    end
    
    if num == 11
    for i = 1 : (200 - 1)
        ad_fifo11(i) = ad_fifo11(i+1);
    end
    ad_fifo11(200) = in;
    out = ad_fifo11;
    end
    
    if num == 12
    for i = 1 : (200 - 1)
        ad_fifo12(i) = ad_fifo12(i+1);
    end
    ad_fifo12(200) = in;
    out = ad_fifo12;
    end
    
    if num == 13
    for i = 1 : (200 - 1)
        ad_fifo13(i) = ad_fifo13(i+1);
    end
    ad_fifo13(200) = in;
    out = ad_fifo13;
    end
    
    if num == 14
    for i = 1 : (200 - 1)
        ad_fifo14(i) = ad_fifo14(i+1);
    end
    ad_fifo14(200) = in;
    out = ad_fifo14;
    end
    
    if num == 15
    for i = 1 : (200 - 1)
        ad_fifo15(i) = ad_fifo15(i+1);
    end
    ad_fifo15(200) = in;
    out = ad_fifo15;
    end
    
    if num == 16
    for i = 1 : (200 - 1)
        ad_fifo16(i) = ad_fifo16(i+1);
    end
    ad_fifo16(200) = in;
    out = ad_fifo16;
    end
end