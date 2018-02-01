fid = fopen('LIDAR_data_14.txt');
C = textscan(fid, '%s', 'Delimiter', '\n', 'CommentStyle', '%');
fclose(fid);
cell = C{1};
[lines,a] =size(cell);
close all
for i=1 : lines/2
   
   
    x = str2num(cell{(i*3-2),1});
    y = str2num(cell{(i*3-1),1});
    [a,numVals] = size(x);
    
    data = horzcat(x', y');
    
    str = sprintf('Measurement %d', i);
    figure('Name', str, 'NumberTitle', 'off', 'position', [100, 100, 900, 900])
    axis([-500 3000 -500 2000])
    hold on
    
    degree=-125;
    xCoord = 430;
    yCoord = 520;
    
    scatter(xCoord,yCoord, 250, 'filled', 'p')
      a=85; a_rad=((a*pi)./180); 
 [THETA,R] = cart2pol(x,y); %Convert to polar coordinates
 THETA=THETA+a_rad; %Add a_rad to theta
 [xr,yr] = pol2cart(THETA,R); %Convert back to Cartesian coordinates

 %plot(x+xCoord,y+yCoord,'g-');  %Original
 %plot(xr,yr,'b-');  %Rotated
 
    x=xr+xCoord+50
    y=yr+yCoord
    %for i= 1 : numVals
     %   
      
     %x(i) = x(i) * cosd(degree) - y(i) * sind(degree);
      
     %y(i) = x(i) * sind(degree) + y(i) * cosd(degree);
      %  x(i)= x(i)+xCoord;
       % y(i)= y(i)+yCoord;
        
    %end
    
    usf=520;
    usb=2320;
    usf_x = 0 * cosd(-degree) - usf * sind(-degree);
    usf_y = 0 * sind(-degree) +  usf* cosd(-degree);
    
    usb_x = 0 * cosd(-degree) - usb * sind(-degree);
    usb_y = 0 * sind(-degree) +  usb* cosd(-degree);
    
    scatter(usf_x+xCoord, usf_y+yCoord, 100, 'x', 'LineWidth',1.5)
    scatter(xCoord-usb_x, yCoord-usb_y, 100, 'x', 'LineWidth',1.5)
    
    direction_x = 0 * cosd(-degree) - 150 * sind(-degree);
    direction_y = 0 * sind(-degree) +  150 * cosd(-degree);
    
    drawArrow = quiver( xCoord,yCoord,direction_x,direction_y, 0, 'linewidth',1,'MaxHeadSize',2);  
    
    plot(x,y)
    
    iterateNeighbor=15;
    for i =1 : iterateNeighbor
        xMean = ones(1,length(x) - 1);
        yMean = ones(1,length(y) - 1);
        for index = 2:length(x)
        xMean(index-1) = mean(x(index-1:index));
        yMean(index-1) = mean(y(index-1:index));
        end
    x=xMean;
    y=yMean;
    end

    %plot(xMean,yMean)

    iterateGroup=2;
    groupSize=5;
    for i= 1 : iterateGroup
        xGroupMean = ones(1,floor(length(x)/groupSize));
        yGroupMean = ones(1,floor(length(y)/groupSize));
        for index = 1 : length(x)/groupSize
            startIndex = (index-1)*groupSize +1;
        
            xGroupMean(index) = mean(x(startIndex : index * groupSize));
            yGroupMean(index) = mean(y(startIndex : index * groupSize));
        end

        x = xGroupMean;
        y = yGroupMean; 
    end
    plot(xGroupMean,yGroupMean)
    
    hold off
    h = legend('Position', 'Ultrasound front', 'Ultrasound back', 'Direction', 'Lidar (raw)', 'Lidar (filtered)');
    set(h,'FontSize',12); 
end





%c =polyfit(x,y,50);
%c = smooth(data, 20, 'rlowess');
%b = smooth(x, 50, 'rlowess');


%plot(c)



%plot(b)
%filt_len = 200;
%a = 1;
%b = (1/filt_len)*ones(1,filt_len);
%mov_avg = filter(b,a,c);

%hold on
%plot(mov_avg,'r','LineWidth',2)

%https://www.mathworks.com/matlabcentral/fileexchange/27783-fitting-data-with-a-sudden-discontinuity?focused=6788302&tab=example&requestedDomain=www.mathworks.com&nocookie=true