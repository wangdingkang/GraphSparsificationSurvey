folder = 'asps/';
files = dir(strcat(folder, '*.txt'));

colors = [0 0 0; 1 0 0; 0.8 0 0; 0.6 0 0; 0.4 0 0; 0 1 0; 0 0.8 0; 0 0.6 0; 0 0.4 0; 0 0 1; 0 0 0.8; 0 0 0.6; 0 0 0.4; 0 1 1; 0 0.8 0.8; 0 0.6 0.6; 0 0.4 0.4];

for i = 1 : length(files)
    filename = files(i).name;
    
    fid = fopen(strcat(folder, filename));
    hFig = figure;
    acFig = figure;
    set(hFig, 'Position', [0 0 2000 1200]);
    set(acFig, 'Position', [1000 0 2000 1200]);
    
    tline = fgetl(fid);
    
    base_histo = zeros(1, 26);
    base_cum = zeros(1, 26);
    ci = 1;
    while ischar(tline)
        C = strsplit(tline);
        s = size(C, 2);
        str = C(1, s);
        str = str{1, 1};
        
        data = str2double(C(1, 1:(s-1)));
        
        if strcmp(str, 'LCC')
            base_histo = data;
            base_cum = cumsum(data);
        else
            % str
            % corrcoef(base_histo, data)
            % corrcoef(base_cum, cumsum(data))
            % norm(base_histo - data)
            % norm(base_cum - cumsum(data))
        end
        
        color = colors(ci, :);
        ci = ci + 1;
        
        figure(hFig);
        hold on;
        plot(0:1:(s-2), data, 'o-', 'DisplayName', str, 'Color', color);
        
        figure(acFig);
        hold on;
        plot(0:1:(s-2), cumsum(data), 'o-', 'DisplayName', str, 'Color', color);
        
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

