input = 'asps/';
files = dir(strcat(input, '*ALL.txt'));
colors = [1 0 0; 0.8 0 0; 0.6 0 0; 0.4 0 0; 0 1 0; 0 0.8 0; 0 0.6 0; 0 0.4 0 0 0 1; 0 0 0.8; 0 0 0.6 0 0 0.4;1 1 0; 0.8 0.8 0; 0.6 0.6 0; 0.4 0.4 0];


tot_size = 16;

datum = [];
lens = size(colors);
distances = [];

for i = 1 : length(files)
    filename = files(i).name;
    
    fid = fopen(strcat(input, filename));
    hFig = figure;
    acFig = figure;
    set(hFig, 'Position', [0 0 2000 1200]);
    set(acFig, 'Position', [1000 0 2000 1200]);
    
    tline = fgetl(fid);
    
    base_histo = zeros(1, 26);
    base_cum = zeros(1, 26);
    ci = 1;
    marker = 'o-';
    while ischar(tline)
        C = strsplit(tline);
        s = size(C, 2);
        str = C(1, s);
        str = str{1, 1};
        
        data = str2double(C(1, 1:(s-1)));
        datum = [datum; data];
        
        if ci > tot_size
            ci = 1;
            marker = 'x-';
        end
        color = colors(ci, :);
        
        if strcmp(marker, 'x-') == 1
            distances = [distances; norm(datum(ci, :) ./ norm(datum(ci, :)) - data ./ norm(data))];
        end
        
        
        figure(hFig);
        hold on;
        plot(0:1:(s-2), data, marker, 'DisplayName', str, 'Color', color);
        
        figure(acFig);
        hold on;
        plot(0:1:(s-2), cumsum(data), marker, 'DisplayName', str, 'Color', color);
        
        ci = (ci + 1);
        tline = fgetl(fid);
    end
    
    figure(hFig);
    l = legend('show');
    t = title({filename; ' '});
    set(l, 'Interpreter', 'none');
    set(t, 'Interpreter', 'none');
    % set(l, 'Location', 'northwest');
    print(hFig, strcat(regexprep(filename , '.txt', ''), '_histo'), '-dpng');
    
    figure(acFig);
    l = legend('show');
    t = title({filename; ' '});
    set(l, 'Interpreter', 'none');
    set(t, 'Interpreter', 'none');
    print(acFig, strcat(regexprep(filename , '.txt', ''), '_accu'), '-dpng');
    
    hold off;
    close(hFig);
    close(acFig);
end
fclose('all');

