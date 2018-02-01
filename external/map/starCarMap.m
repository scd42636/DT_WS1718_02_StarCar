%set figure
 close all
 str = sprintf('StarCar');
 figure('Name', str, 'NumberTitle', 'off', 'position', [0, 0, 800, 800])
 axis([-3000 3000 -3000 3000])


go = true;
while go
%acelerate X
fid = fopen('BeschleunigungXCp.txt');
C = textscan(fid, '%s', 'Delimiter', '\n', 'CommentStyle', '%');
fclose(fid);
cell =  C{1};
[lines,a] = size(cell);
acelerateX = str2num(cell{lines,1});
%acelerate Y
fid = fopen('BeschleunigungYCp.txt');
C = textscan(fid, '%s', 'Delimiter', '\n', 'CommentStyle', '%');
fclose(fid);
cell =  C{1};
[lines,a] = size(cell);
acelerateY = str2num(cell{lines,1});
%compass
fid = fopen('CompassCp.txt');
C = textscan(fid, '%s', 'Delimiter', '\n', 'CommentStyle', '%');
fclose(fid);
cell =  C{1};
[lines,a] = size(cell);
compass = str2num(cell{lines,1});
%ultra back
fid = fopen('UltraHintenCp.txt');
C = textscan(fid, '%s', 'Delimiter', '\n', 'CommentStyle', '%');
fclose(fid);
cell =  C{1};
[lines,a] = size(cell);
usBack = str2num(cell{lines,1});
%ultra front
fid = fopen('UltraVorneCp.txt');
C = textscan(fid, '%s', 'Delimiter', '\n', 'CommentStyle', '%');
fclose(fid);
cell =  C{1};
[lines,a] = size(cell);
usFront = str2num(cell{lines,1});
%lidar
fid = fopen('LidarCp.txt');
C = textscan(fid, '%s', 'Delimiter', '\n', 'CommentStyle', '%');
fclose(fid);
cell = C{1};
[lines,a] =size(cell);
liadr=cell;


%calculate 
    clf
    hold on
    ax = gca;
    ax.ColorOrderIndex = 1;
    
    % Compass
    direction_x = 0 * cosd(-compass) - 200 * sind(-compass);
    direction_y = 0 * sind(-compass) +  200 * cosd(-compass);
    
    drawArrow = quiver( 0,0,direction_x,direction_y, 0, 'linewidth',1,'MaxHeadSize',2);  

    %Ultra Sound
    usf_x = 0 * cosd(-compass) - (usFront*10) * sind(-compass);
    usf_y = 0 * sind(-compass) + (usFront*10) * cosd(-compass);
    
    usb_x = 0 * cosd(-compass) - (usBack*10) * sind(-compass);
    usb_y = 0 * sind(-compass) +  (usBack*10) * cosd(-compass);
    
    scatter(usf_x, usf_y, 100, 'x', 'LineWidth',1.5)
    scatter(-usb_x, -usb_y, 100, 'x', 'LineWidth',1.5)
    
    %Lidar
    lidarX = str2num(cell{(2),1});
    lidarY = str2num(cell{(3),1});
    [a,numVals] = size(lidarX);
    
    lidarData = horzcat(lidarX', lidarY');
    compass_rad=((-compass*pi)./180); 
    [THETA,R] = cart2pol(lidarX,lidarY); %Convert to polar coordinates
    THETA=THETA+compass_rad; %Add a_rad to theta
    
    [lidarXrotated,lidarYrotated] = pol2cart(THETA,R);
    plot(lidarXrotated,lidarYrotated)
    
    iterateNeighbor=15;
    for i =1 : iterateNeighbor
        xMean = ones(1,length(lidarXrotated) - 1);
        yMean = ones(1,length(lidarYrotated) - 1);
        for index = 2:length(lidarXrotated)
        xMean(index-1) = mean(lidarXrotated(index-1:index));
        yMean(index-1) = mean(lidarYrotated(index-1:index));
        end
    lidarXrotated=xMean;
    lidarYrotated=yMean;
    end
    
    iterateGroup=2;
    groupSize=5;
    for i= 1 : iterateGroup
        xGroupMean = ones(1,floor(length(lidarXrotated)/groupSize));
        yGroupMean = ones(1,floor(length(lidarYrotated)/groupSize));
        for index = 1 : length(lidarXrotated)/groupSize
            startIndex = (index-1)*groupSize +1;
        
            xGroupMean(index) = mean(lidarXrotated(startIndex : index * groupSize));
            yGroupMean(index) = mean(lidarYrotated(startIndex : index * groupSize));
        end
    end
    plot(xGroupMean,yGroupMean)
    
    hold off
    
    %Legend
    h = legend('Compass', 'Ultrasound front', 'Ultrasound back', 'Lidar (raw)', 'Lidar (filtered)');
    set(h,'FontSize',12); 
    
    %Acceleration
    dim = [.2 .5 .3 .3];
    str = {['Acelleration X = ' num2str(acelerateX,'%d')], ['Acelleration Y = ' num2str(acelerateY,'%d')]};
    annotation('textbox',dim,'String',str,'FitBoxToText','on');
    pause(0.5)
end