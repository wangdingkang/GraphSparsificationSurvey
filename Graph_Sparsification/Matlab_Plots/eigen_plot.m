folder = 'eigens/';
files = dir(strcat(folder, '*.txt'));

for i = 1 : length(files)
    filename = files(i).name;
    
    fid = fopen(strcat(folder, filename));
    hFig = figure();
    axh = axes('Parent', hFig);
    set(hFig, 'Position', [0 0 2000 1200]);
    
    hold on;
    tline = fgetl(fid);
    while ischar(tline)
        C = strsplit(tline);
        s = size(C, 2);
        str = C(1, s);
        str = str{1, 1};
        plot(1:1:(s-1), str2double(C(1, 1:(s-1))), 'o-', 'DisplayName', str);
        
        tline = fgetl(fid);
    end
    
    fclose(fid);
    l = legend('show');
    t = title({filename; ' '});
    set(l, 'Interpreter', 'none');
    set(t, 'Interpreter', 'none');
    set(l, 'Location', 'northwest');
    print(hFig, regexprep(filename , '.txt', ''), '-dpng');
    hold off;
    close(hFig);
    
end

