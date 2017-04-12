folder = 'asps/';
files = dir(strcat(folder, '*.txt'));

for i = 1 : length(files)
    filename = files(i).name;
    
    fid = fopen(strcat(folder, filename));
    hFig = figure;
    acFig = figure;
    set(hFig, 'Position', [0 0 2000 1200]);
    set(acFig, 'Position', [1000 0 2000 1200]);
    
    tline = fgetl(fid);
    while ischar(tline)
        C = strsplit(tline);
        s = size(C, 2);
        str = C(1, s);
        str = str{1, 1};
        
        data = str2double(C(1, 1:(s-2)));
        
        figure(hFig);
        hold on;
        plot(1:1:(s-2), data, 'o-', 'DisplayName', str);
        
        figure(acFig);
        hold on;
        plot(1:1:(s-2), cumsum(data), 'o-', 'DisplayName', str);
        
        tline = fgetl(fid);
    end
    
    fclose(fid);
    
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

