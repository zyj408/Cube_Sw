function varargout = untitled(varargin)
% UNTITLED MATLAB code for untitled.fig
%      UNTITLED, by itself, creates a new UNTITLED or raises the existing
%      singleton*.
%
%      H = UNTITLED returns the handle to a new UNTITLED or the handle to
%      the existing singleton*.
%
%      UNTITLED('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in UNTITLED.M with the given input arguments.
%
%      UNTITLED('Property','Value',...) creates a new UNTITLED or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before untitled_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to untitled_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help untitled

% Last Modified by GUIDE v2.5 03-Jun-2015 05:17:32

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @untitled_OpeningFcn, ...
                   'gui_OutputFcn',  @untitled_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

function SerialUpdate(obj,events,handles)
    
time_temp = datestr(now, 13);
set(handles.edit17, 'String', num2str(time_temp));
scom = getappdata(handles.figure1, 'uart');

data_temp = [235 80 03 04 00 00 00 00 00];
clear_fifo(scom);
fwrite(scom, data_temp, 'char');
setappdata(handles.figure1, 'tx_cnt', getappdata(handles.figure1, 'tx_cnt') + 1); 
set(handles.edit19, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
set(handles.text9, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
pause(0.1);
        if get(scom, 'BytesAvailable') == 222 %接收到7个数据
            data_rx = fread(scom, 222, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 49
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                set(handles.edit18, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
                set(handles.text10, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
                set(handles.rx_cmd_cnt, 'String', num2str(data_rx(5)+ data_rx(6)*256));
                set(handles.time_on_site, 'String', [num2str(data_rx(8)) ':' num2str(data_rx(9)) ':' num2str(data_rx(10))]);
                set(handles.tel_addr, 'String', num2str(data_rx(11)+data_rx(12)*256+data_rx(13)*256*256+data_rx(14)*256*256*256));
                set(handles.gps_addr, 'String', num2str(data_rx(15)+data_rx(16)*256+data_rx(17)*256*256+data_rx(18)*256*256*256));
                set(handles.pwm_1, 'String', num2str(data_rx(19)+data_rx(20)*256));
                set(handles.pwm_2, 'String', num2str(data_rx(21)+data_rx(22)*256))
                if data_rx(23) == 255
                    set(handles.bat_stat, 'String', '正常工作');
                    set(handles.bat_cha, 'String', '正在放电');
                    set(handles.bat_bro, 'String', '92%');
                    set(handles.bat_heat, 'String', '未加热');
                    set(handles.bat_heat_mode, 'String', '手动加热');
                    set(handles.bat_init, 'String', num2str(data_rx(31)*256^3+data_rx(30)*256^2+data_rx(29)*256^1+data_rx(28)));
                    set(handles.bat_per, 'String', [num2str(data_rx(32) + data_rx(33)*256) '%']);
                end
                if data_rx(44) == 255
                    if data_rx(45) == 0
                        set(handles.text67, 'String', '未充电');
                    else
                        set(handles.text67, 'String', '正在充电');
                    end
                    if data_rx(46) == 0
                        set(handles.text68, 'String', '未充电');
                    else
                        set(handles.text68, 'String', '正在充电');
                    end                    
                    if data_rx(47) == 0
                        set(handles.text69, 'String', '未充电');
                    else
                        set(handles.text69, 'String', '正在充电');
                    end                    
                    if data_rx(48) == 0
                        set(handles.text70, 'String', '未充电');
                    else
                        set(handles.text70, 'String', '正在充电');
                    end                    
                    if data_rx(49) == 0
                        set(handles.text71, 'String', '未充电');
                    else
                        set(handles.text71, 'String', '正在充电');
                    end                    
                    if data_rx(50) == 0
                        set(handles.text72, 'String', '未充电');
                    else
                        set(handles.text72, 'String', '正在充电');
                    end                        
                    if data_rx(51) == 0
                        set(handles.text75, 'String', '关闭');
                    else
                        set(handles.text75, 'String', '正在工作');
                    end
                    if data_rx(52) == 0
                        set(handles.text76, 'String', '关闭');
                    else
                        set(handles.text76, 'String', '正在工作');
                    end                    
                end
                
                if data_rx(157) == 255
                    set(handles.text77, 'String', [num2str(data_rx(158)+data_rx(159)*256) 'mA']);
                    set(handles.text78, 'String', [num2str(data_rx(160)+data_rx(161)*256) 'mA']);
                    set(handles.text79, 'String', [num2str(data_rx(162)+data_rx(163)*256) 'mA']);
                    set(handles.text80, 'String', [num2str(data_rx(164)+data_rx(165)*256) 'mA']);
                    set(handles.text81, 'String', [num2str(data_rx(166)+data_rx(167)*256) 'mA']);
                    set(handles.text82, 'String', [num2str(data_rx(168)+data_rx(169)*256) 'mA']);
                    
                    set(handles.text83, 'String', [num2str(data_rx(170)+data_rx(171)*256) 'mV']);
                    set(handles.text84, 'String', [num2str(data_rx(172)+data_rx(173)*256) 'mV']);
                    set(handles.text85, 'String', [num2str(data_rx(174)+data_rx(175)*256) 'mV']);
                    set(handles.text86, 'String', [num2str(data_rx(176)+data_rx(177)*256) 'mV']);
                    set(handles.text87, 'String', [num2str(data_rx(178)+data_rx(179)*256) 'mV']);
                    set(handles.text88, 'String', [num2str(data_rx(180)+data_rx(181)*256) 'mV']);
                    
                    set(handles.text90, 'String', [num2str(data_rx(182)+data_rx(183)*256) 'mA']);
                    c_bus = data_rx(184)+data_rx(185)*256;
                    v_bus = data_rx(186)+data_rx(187)*256;
                    set(handles.text91, 'String', ['母线电流：' num2str(c_bus) 'mA']);
                    set(handles.text92, 'String', ['母线电压：' num2str(v_bus) 'mV']);
                    b1 = pushfifo(c_bus,1); 
                    b2 = pushfifo(v_bus,2); 
                    plot(handles.axes1,b1);
                    set(handles.axes1, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 100]);
                    plot(handles.axes2,b2);
                    set(handles.axes2, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 8000]);
                    c_1 = data_rx(188)+data_rx(189)*256;
                    c_2 = data_rx(190)+data_rx(191)*256;
                    c_3 = data_rx(192)+data_rx(193)*256;
                    c_4 = data_rx(194)+data_rx(195)*256;
                    c_5 = data_rx(196)+data_rx(197)*256;
                    c_6 = data_rx(198)+data_rx(199)*256;
                    set(handles.text93, 'String', ['输出电流1：' num2str(c_1) 'mA']);
                    set(handles.text94, 'String', ['输出电流2：' num2str(c_2) 'mA']);
                    set(handles.text95, 'String', ['输出电流3：' num2str(c_3) 'mA']);
                    set(handles.text96, 'String', ['输出电流4：' num2str(c_4) 'mA']);
                    set(handles.text97, 'String', ['输出电流5：' num2str(c_5) 'mA']);
                    set(handles.text98, 'String', ['输出电流6：' num2str(c_6) 'mA']);
                    
                    b3 = pushfifo(c_1,3); 
                    b4 = pushfifo(c_2,4);                     
                    b5 = pushfifo(c_3,5); 
                    b6 = pushfifo(c_4,6); 
                    b7 = pushfifo(c_5,7); 
                    b8 = pushfifo(c_6,8); 
                    
                    plot(handles.axes6,b3);
                    set(handles.axes6, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 200]);
                    plot(handles.axes7,b4);
                    set(handles.axes7, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 200]);
                    plot(handles.axes8,b5);
                    set(handles.axes8, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 200]);
                    plot(handles.axes9,b6);
                    set(handles.axes9, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 200]);
                    plot(handles.axes10,b7);
                    set(handles.axes10, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 200]);
                    plot(handles.axes11,b8);
                    set(handles.axes11, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 200]);
                    
                    t_1 = data_rx(210)+data_rx(211)*256;
                    t_2 = data_rx(212)+data_rx(213)*256;
                    t_3 = data_rx(214)+data_rx(215)*256;
                    t_4 = data_rx(216)+data_rx(217)*256;
                    t_5 = data_rx(218)+data_rx(219)*256;
                    t_6 = data_rx(220)+data_rx(221)*256;
                    set(handles.text103, 'String', [num2str(t_1) 'C']);
                    set(handles.text104, 'String', [num2str(t_2) 'C']);
                    set(handles.text105, 'String', [num2str(t_3) 'C']);
                    set(handles.text106, 'String', [num2str(t_4) 'C']);
                    
                    set(handles.text109, 'String', ['27' 'C']);
                    set(handles.text110, 'String', ['27' 'C']);
                    set(handles.text113, 'String', '未连接');
                    set(handles.text114, 'String', '无错误');
                    cpu = 15 + 5*rand();
                    set(handles.edit29, 'String', num2str(cpu));
                    set(handles.edit30, 'String', '430KB/2GB');
                end
                
            end
        end
        
    
% --- Executes just before untitled is made visible.
function untitled_OpeningFcn(hObject, eventdata, handles, varargin)

handles.output = hObject;


setappdata(handles.figure1, 'connect_stat',0);
setappdata(handles.figure1, 'rx_cnt',0);
setappdata(handles.figure1, 'tx_cnt',0);


% Update handles structure
guidata(hObject, handles);

% UIWAIT makes untitled wait for user response (see UIRESUME)
% uiwait(handles.figure1);


function clear_fifo(uart_handle)
if get(uart_handle, 'BytesAvailable') ~= 0
    fread(uart_handle, get(uart_handle, 'BytesAvailable'), 'char');
end

% --- Outputs from this function are returned to the command line.
function varargout = untitled_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in popupmenu1.
function popupmenu1_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu1


% --- Executes during object creation, after setting all properties.
function popupmenu1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu2.
function popupmenu2_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu2


% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in togglebutton1.
function togglebutton1_Callback(hObject, eventdata, handles)
global timer1;
if get(hObject, 'value')
    com_n = sprintf('com%d',get(handles.popupmenu1, 'value'));
    scom = serial(com_n);
    set(scom, 'BaudRate', 115200, 'Parity', 'none', 'DataBits',...
        8, 'StopBits', 1, 'BytesAvailableFcnMode', 'byte');
    
    try
        fopen(scom);
    catch
        msgbox('串口无法打开！');
        %set(hObject, 'string', '关闭串口');
        set(hObject, 'value', 0);
        fclose(instrfind);
        delete(instrfind);
        return;
    end

    set(hObject, 'string', '关闭串口');
    set(handles.text6, 'string', '已连接');
    setappdata(handles.figure1, 'connect_stat',1);
    setappdata(handles.figure1, 'uart',scom);
    set(handles.pushbutton10, 'Enable', 'on');
    setappdata(handles.figure1, 'rx_cnt',0);
    setappdata(handles.figure1, 'tx_cnt',0);
        timer1 = timer('Period',1,'ExecutionMode','FixedRate','TimerFcn',{@SerialUpdate,handles});
    start(timer1);
else
    t = timerfind;
    try
        stop(t);
        delete(t);
        clear t
    end
    
    scom = getappdata(handles.figure1, 'uart');
    stopasync(scom);
    fclose(scom);
    delete(scom);
    set(hObject, 'string', '打开串口');
    set(handles.text6, 'string', '未连接');
    set(handles.text9, 'string', '0');
    set(handles.text10, 'string', '0');
    setappdata(handles.figure1, 'connect_stat',0);
    set(handles.pushbutton10, 'Enable', 'off');
end

function uart_switch(handles, opt)

if getappdata(handles.figure1, 'connect_stat')
    scom = getappdata(handles.figure1, 'uart');
    data_temp = [235 80 opt 04 00 00 00 00 00];
    clear_fifo(scom);
    fwrite(scom, data_temp, 'char');
    setappdata(handles.figure1, 'tx_cnt', getappdata(handles.figure1, 'tx_cnt') + 1); 
    set(handles.edit19, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
    set(handles.text9, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));

    while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                set(handles.edit18, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
                set(handles.text10, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
            end
            break;
        end
        pause(0.001);
    end
end


% --- Executes on button press in radiobutton1.
function radiobutton1_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 16);
else
    uart_switch(handles, 17);
end

% --- Executes on button press in radiobutton2.
function radiobutton2_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 18);
else
    uart_switch(handles, 19);
end


% --- Executes on button press in radiobutton3.
function radiobutton3_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 20);
else
    uart_switch(handles, 21);
end


% --- Executes on button press in radiobutton4.
function radiobutton4_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 24);
else
    uart_switch(handles, 25);
end


% --- Executes on button press in radiobutton5.
function radiobutton5_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 26);
else
    uart_switch(handles, 27);
end


% --- Executes on button press in radiobutton6.
function radiobutton6_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 28);
else
    uart_switch(handles, 29);
end


% --- Executes on button press in radiobutton7.
function radiobutton7_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 30);
else
    uart_switch(handles, 31);
end


% --- Executes on button press in radiobutton8.
function radiobutton8_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 22);
else
    uart_switch(handles, 23);
end


% --- Executes on button press in radiobutton9.
function radiobutton9_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 32);
else
    uart_switch(handles, 33);
end


% --- Executes on button press in radiobutton10.
function radiobutton10_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 34);
else
    uart_switch(handles, 35);
end


% --- Executes on button press in radiobutton11.
function radiobutton11_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 36);
else
    uart_switch(handles, 37);
end


% --- Executes on button press in radiobutton12.
function radiobutton12_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 38);
else
    uart_switch(handles, 39);
end


% --- Executes on button press in togglebutton2.
function togglebutton2_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 41);
    set(hObject, 'String', '磁棒1反转');
else
    uart_switch(handles, 40);
    set(hObject, 'String', '磁棒1正转');
end


% --- Executes on button press in togglebutton3.
function togglebutton3_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 43);
    set(hObject, 'String', '磁棒2反转');
else
    uart_switch(handles, 42);
    set(hObject, 'String', '磁棒2正转');
end


% --- Executes on button press in togglebutton4.
function togglebutton4_Callback(hObject, eventdata, handles)
if get(hObject, 'Value')
    uart_switch(handles, 45);
    set(hObject, 'String', '磁棒3反转');   
else
    uart_switch(handles, 44);
    set(hObject, 'String', '磁棒3正转');    
end



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit3_Callback(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit3 as text
%        str2double(get(hObject,'String')) returns contents of edit3 as a double


% --- Executes during object creation, after setting all properties.
function edit3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit5_Callback(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit5 as text
%        str2double(get(hObject,'String')) returns contents of edit5 as a double


% --- Executes during object creation, after setting all properties.
function edit5_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit6_Callback(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit6 as text
%        str2double(get(hObject,'String')) returns contents of edit6 as a double


% --- Executes during object creation, after setting all properties.
function edit6_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit7_Callback(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit7 as text
%        str2double(get(hObject,'String')) returns contents of edit7 as a double


% --- Executes during object creation, after setting all properties.
function edit7_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        data_temp = 255;
        uart_inject_data(handles, 49, data_temp);
    end

% --- Executes on button press in pushbutton4.
function pushbutton4_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        data_temp = 255;
        uart_inject_data(handles, 84, data_temp);
    end

% --- Executes on button press in pushbutton5.
function pushbutton5_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        data_temp = 255;
        uart_inject_data(handles, 85, data_temp);
    end


% --- Executes on button press in pushbutton7.
function pushbutton7_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        data_temp = 255;
        uart_inject_data(handles, 86, data_temp);
    end


function edit8_Callback(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit8 as text
%        str2double(get(hObject,'String')) returns contents of edit8 as a double


% --- Executes during object creation, after setting all properties.
function edit8_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit8 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit9_Callback(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit9 as text
%        str2double(get(hObject,'String')) returns contents of edit9 as a double


% --- Executes during object creation, after setting all properties.
function edit9_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit9 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit10_Callback(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit10 as text
%        str2double(get(hObject,'String')) returns contents of edit10 as a double


% --- Executes during object creation, after setting all properties.
function edit10_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit10 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit11_Callback(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit11 as text
%        str2double(get(hObject,'String')) returns contents of edit11 as a double


% --- Executes during object creation, after setting all properties.
function edit11_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit11 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit12_Callback(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit12 as text
%        str2double(get(hObject,'String')) returns contents of edit12 as a double


% --- Executes during object creation, after setting all properties.
function edit12_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit12 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit13_Callback(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit13 as text
%        str2double(get(hObject,'String')) returns contents of edit13 as a double


% --- Executes during object creation, after setting all properties.
function edit13_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit14_Callback(hObject, eventdata, handles)
% hObject    handle to edit14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit14 as text
%        str2double(get(hObject,'String')) returns contents of edit14 as a double


% --- Executes during object creation, after setting all properties.
function edit14_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit14 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit15_Callback(hObject, eventdata, handles)
% hObject    handle to edit15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit15 as text
%        str2double(get(hObject,'String')) returns contents of edit15 as a double


% --- Executes during object creation, after setting all properties.
function edit15_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit15 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton8.
function pushbutton8_Callback(hObject, eventdata, handles)

    pushbutton15_Callback(hObject, eventdata, handles);
    if getappdata(handles.figure1, 'connect_stat')
            data_temp(1) = 255;
     
        p_value = floor(str2double(get(handles.edit9, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(1 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        
         p_value = floor(str2double(get(handles.edit10, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(5 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        
                p_value = floor(str2double(get(handles.edit11, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(9 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        
                p_value = floor(str2double(get(handles.edit12, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(13 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        
                p_value = floor(str2double(get(handles.edit8, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(17 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        
                p_value = floor(str2double(get(handles.edit13, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(21 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        
                p_value = floor(str2double(get(handles.edit14, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(25 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
                p_value = floor(str2double(get(handles.edit15, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(29 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        
        uart_inject_data(handles, 90, data_temp);
    end


% --- Executes on button press in radiobutton18.
function radiobutton18_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton18 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton18


% --- Executes on button press in radiobutton19.
function radiobutton19_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton19 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton19


% --- Executes on button press in radiobutton20.
function radiobutton20_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton20 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton20



function edit17_Callback(hObject, eventdata, handles)
% hObject    handle to edit17 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit17 as text
%        str2double(get(hObject,'String')) returns contents of edit17 as a double


% --- Executes during object creation, after setting all properties.
function edit17_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit17 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit18_Callback(hObject, eventdata, handles)
% hObject    handle to edit18 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit18 as text
%        str2double(get(hObject,'String')) returns contents of edit18 as a double


% --- Executes during object creation, after setting all properties.
function edit18_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit18 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function uart_inject_data(handles, opt, data)
    xor_checksum = 0;
    deta_length = size(data,2);
    for i=1:deta_length
        xor_checksum=bitxor(xor_checksum,data(i), 'uint8');   
    end
    
    scom = getappdata(handles.figure1, 'uart');        
    data_temp = [235 80 opt (4+deta_length) 00 00 00 00 data xor_checksum];
    
    clear_fifo(scom);
    fwrite(scom, data_temp, 'char');
    setappdata(handles.figure1, 'tx_cnt', getappdata(handles.figure1, 'tx_cnt') + 1); 
    set(handles.edit19, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
    set(handles.text9, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
        while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                set(handles.edit18, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
                set(handles.text10, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
            end
            break;
        end
        pause(0.001);
        end
% --- Executes on key press with focus on edit5 and none of its controls.
function edit5_KeyPressFcn(hObject, eventdata, handles)



function edit19_Callback(hObject, eventdata, handles)
% hObject    handle to edit19 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit19 as text
%        str2double(get(hObject,'String')) returns contents of edit19 as a double


% --- Executes during object creation, after setting all properties.
function edit19_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit19 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit20_Callback(hObject, eventdata, handles)
% hObject    handle to edit20 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit20 as text
%        str2double(get(hObject,'String')) returns contents of edit20 as a double


% --- Executes during object creation, after setting all properties.
function edit20_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit20 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function uart_ping(handles)
if getappdata(handles.figure1, 'connect_stat')
    scom = getappdata(handles.figure1, 'uart');
    data_temp = [235 80 01 04 00 00 00 00 00];
        
    clear_fifo(scom);
    fwrite(scom, data_temp, 'char');
    setappdata(handles.figure1, 'tx_cnt', getappdata(handles.figure1, 'tx_cnt') + 1); 
    set(handles.edit19, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
    set(handles.text9, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
    
    
    
    while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                set(handles.edit18, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
                set(handles.text10, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
            end
            break;
        end
        pause(0.001);
    end
end

% --- Executes on button press in pushbutton10.
function pushbutton10_Callback(hObject, eventdata, handles)
    uart_ping(handles);

    
    
    
    
function uart_inject_pwm(handles, opt, val)
if getappdata(handles.figure1, 'connect_stat')
    scom = getappdata(handles.figure1, 'uart');
    %val = str2num(val);
    data_temp = [235 80 opt 05 00 00 00 00 val val];

    clear_fifo(scom);
    fwrite(scom, data_temp, 'char');
    setappdata(handles.figure1, 'tx_cnt', getappdata(handles.figure1, 'tx_cnt') + 1); 
    set(handles.edit19, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
    set(handles.text9, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));

    while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                set(handles.edit18, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
                set(handles.text10, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
            end
            break;
        end
        pause(0.001);
    end
end

% --- Executes on key press with focus on edit1 and none of its controls.
function edit1_KeyPressFcn(hObject, eventdata, handles)
enter_key=eventdata.Key;
    if strcmp(enter_key, 'return')
        pwm_temp = str2num(get(hObject, 'String'));
        if pwm_temp <= 100
            uart_inject_pwm(handles, 91, pwm_temp);
        end
    end


% --- Executes on key press with focus on edit2 and none of its controls.
function edit2_KeyPressFcn(hObject, eventdata, handles)
enter_key=eventdata.Key;
    if strcmp(enter_key, 'return')
        pwm_temp = str2num(get(hObject, 'String'));
        if pwm_temp <= 100
            uart_inject_pwm(handles, 92, pwm_temp);
        end
    end


% --- Executes on key press with focus on edit3 and none of its controls.
function edit3_KeyPressFcn(hObject, eventdata, handles)
enter_key=eventdata.Key;
    if strcmp(enter_key, 'return')
        pwm_temp = str2num(get(hObject, 'String'));
        if pwm_temp <= 100
            uart_inject_pwm(handles, 93, pwm_temp);
        end
    end


% --- Executes on button press in pushbutton11.
function pushbutton11_Callback(hObject, eventdata, handles)
if getappdata(handles.figure1, 'connect_stat')
    time_temp = clock;
    time_temp(1) = time_temp(1) - 2000;
    time_temp(6) = floor(time_temp(6));
    
    xor_checksum = 0;
           
    for i=1:6
        xor_checksum=bitxor(xor_checksum,time_temp(i),'int8');   
    end
    scom = getappdata(handles.figure1, 'uart');        
    data_temp = [235 80 100 10 00 00 00 00 time_temp(1) time_temp(2) time_temp(3) time_temp(4) time_temp(5) time_temp(6) xor_checksum];

    clear_fifo(scom);
    fwrite(scom, data_temp, 'char');
    setappdata(handles.figure1, 'tx_cnt', getappdata(handles.figure1, 'tx_cnt') + 1); 
    set(handles.edit19, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
    set(handles.text9, 'String', num2str(getappdata(handles.figure1, 'tx_cnt')));
        while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                set(handles.edit18, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
                set(handles.text10, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
            end
            break;
        end
        pause(0.001);
        end

end


% --- Executes on button press in pushbutton12.
function pushbutton12_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        p_flag = get(handles.radiobutton18, 'Value');
        if p_flag == 1
            data_temp(1) = 255;
        else
            data_temp(1) = 0;
        end    
        p_value = floor(str2double(get(handles.edit5, 'String')) * 100000);
        
        for i = 1:4
            data_temp(1 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        uart_inject_data(handles, 81, data_temp);
   end


% --- Executes on button press in pushbutton13.
function pushbutton13_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        p_flag = get(handles.radiobutton19, 'Value');
        if p_flag == 1
            data_temp(1) = 255;
        else
            data_temp(1) = 0;
        end    
        p_value = floor(str2double(get(handles.edit6, 'String')) * 100000);
        
        for i = 1:4
            data_temp(1 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        uart_inject_data(handles, 82, data_temp);
    end
   
% --- Executes on button press in pushbutton14.
function pushbutton14_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        p_flag = get(handles.radiobutton20, 'Value');
        if p_flag == 1
            data_temp(1) = 255;
        else
            data_temp(1) = 0;
        end    
        p_value = floor(str2double(get(handles.edit7, 'String')) * 100000);
        
        for i = 1:4
            data_temp(1 + i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        uart_inject_data(handles, 83, data_temp);
    end



function edit21_Callback(hObject, eventdata, handles)
% hObject    handle to edit21 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit21 as text
%        str2double(get(hObject,'String')) returns contents of edit21 as a double


% --- Executes during object creation, after setting all properties.
function edit21_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit21 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton15.
function pushbutton15_Callback(hObject, eventdata, handles)
    if getappdata(handles.figure1, 'connect_stat')
        p_value = floor(str2double(get(handles.edit21, 'String')) * 1000000);
        
        for i = 1:4
            data_temp(i) = floor(p_value /(256^(4-i)));
            p_value = mod(p_value, (256^(4-i)));
        end
        uart_inject_data(handles, 94, data_temp);
    end



function rx_cmd_cnt_Callback(hObject, eventdata, handles)
% hObject    handle to rx_cmd_cnt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of rx_cmd_cnt as text
%        str2double(get(hObject,'String')) returns contents of rx_cmd_cnt as a double


% --- Executes during object creation, after setting all properties.
function rx_cmd_cnt_CreateFcn(hObject, eventdata, handles)
% hObject    handle to rx_cmd_cnt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function time_on_site_Callback(hObject, eventdata, handles)
% hObject    handle to time_on_site (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of time_on_site as text
%        str2double(get(hObject,'String')) returns contents of time_on_site as a double


% --- Executes during object creation, after setting all properties.
function time_on_site_CreateFcn(hObject, eventdata, handles)
% hObject    handle to time_on_site (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function tel_addr_Callback(hObject, eventdata, handles)
% hObject    handle to tel_addr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of tel_addr as text
%        str2double(get(hObject,'String')) returns contents of tel_addr as a double


% --- Executes during object creation, after setting all properties.
function tel_addr_CreateFcn(hObject, eventdata, handles)
% hObject    handle to tel_addr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function gps_addr_Callback(hObject, eventdata, handles)
% hObject    handle to gps_addr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of gps_addr as text
%        str2double(get(hObject,'String')) returns contents of gps_addr as a double


% --- Executes during object creation, after setting all properties.
function gps_addr_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gps_addr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit26_Callback(hObject, eventdata, handles)
% hObject    handle to edit26 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit26 as text
%        str2double(get(hObject,'String')) returns contents of edit26 as a double


% --- Executes during object creation, after setting all properties.
function edit26_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit26 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit27_Callback(hObject, eventdata, handles)
% hObject    handle to edit27 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit27 as text
%        str2double(get(hObject,'String')) returns contents of edit27 as a double


% --- Executes during object creation, after setting all properties.
function edit27_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit27 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function pwm_1_Callback(hObject, eventdata, handles)
% hObject    handle to pwm_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pwm_1 as text
%        str2double(get(hObject,'String')) returns contents of pwm_1 as a double


% --- Executes during object creation, after setting all properties.
function pwm_1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pwm_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function pwm_2_Callback(hObject, eventdata, handles)
% hObject    handle to pwm_2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pwm_2 as text
%        str2double(get(hObject,'String')) returns contents of pwm_2 as a double


% --- Executes during object creation, after setting all properties.
function pwm_2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pwm_2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function text40_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text40 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in pushbutton16.
function pushbutton16_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton16 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton17.
function pushbutton17_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton17 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton18.
function pushbutton18_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton18 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton19.
function pushbutton19_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton19 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton20.
function pushbutton20_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton20 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton21.
function pushbutton21_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton21 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function edit29_Callback(hObject, eventdata, handles)
% hObject    handle to edit29 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit29 as text
%        str2double(get(hObject,'String')) returns contents of edit29 as a double


% --- Executes during object creation, after setting all properties.
function edit29_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit29 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function edit30_Callback(hObject, eventdata, handles)
% hObject    handle to edit30 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit30 as text
%        str2double(get(hObject,'String')) returns contents of edit30 as a double


% --- Executes during object creation, after setting all properties.
function edit30_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit30 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
