folder = 'samples/';
origin_file = 'node_LCC_dumb_graph.txt';
origin = load(origin_file);

files = dir(strcat(folder, '*.txt'));

for i = 1 : length(files)
    
    hFig = figure();
    axh = axes('Parent', hFig);
    set(hFig, 'Position', [0 0 2000 1200])
    ho = scatter3(origin(:, 2), origin(:, 3), origin(:, 4), 80, [0 0 0]);
    hold;
    filename = files(i).name;
    temp = load(strcat(folder, filename));
    ht = scatter3(origin(temp(:) + 1, 2), origin(temp(:) + 1, 3), origin(temp(:) + 1, 4), 50, rand(1,3), 'filled');
    
    view(axh, -22, 20);
    legend(axh, [ho, ht], {origin_file, filename}, 'Interpreter', 'none');
    
    print(hFig, regexprep(strcat('graph', filename), '.txt', ''), '-dpng');
    close(hFig);
end

