 myAudioRecording(5000,5,5000);
 % add an extra half-second so that we get the full duration in our
    % processing
    durationSecs = durationSecs + 0.5;
    % index of the last sample obtained from our recording
    lastSampleIdx = 0;
    % start time of the recording
    atTimSecs     = 0;
    % create the audio recorder
    recorder = audiorecorder(Fs,8,1);
    % assign a timer function to the recorder
    set(recorder,'TimerPeriod',1,'TimerFcn',@audioTimerCallback);
     % create a figure with two subplots
    hFig   = figure;
    hAxes1 = subplot(2,1,1);
    hAxes2 = subplot(2,1,2);
    % create the graphics handles to the data that will be plotted on each
    % axes
    hPlot1 = plot(hAxes1,NaN,NaN);
    hPlot2 = plot(hAxes2,NaN,NaN);
    drawnow;
    % start the recording
    record(recorder,durationSecs);
    