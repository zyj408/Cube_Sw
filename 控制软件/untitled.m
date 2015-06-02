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

% Last Modified by GUIDE v2.5 02-Jun-2015 20:06:48

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
uart_inject_data(scom, 3, 0);

while(1)
    a=1;
end
% --- Executes just before untitled is made visible.
function untitled_OpeningFcn(hObject, eventdata, handles, varargin)
global timer1;
handles.output = hObject;
timer1 = timer('Period',1,'ExecutionMode','FixedRate','TimerFcn',{@SerialUpdate,handles});
start(timer1);

setappdata(handles.figure1, 'connect_stat',0);
setappdata(handles.figure1, 'rx_cnt',0);
setappdata(handles.figure1, 'tx_cnt',0);


% Update handles structure
guidata(hObject, handles);

% UIWAIT makes untitled wait for user response (see UIRESUME)
% uiwait(handles.figure1);


function clear_fifo(uart_handle)
if get(uart_handle, 'BytesAvailable') ~= 0
    fread(scom, get(uart_handle, 'BytesAvailable'), 'char');
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
    

    while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
            end
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
    
        while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
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
    
    
    
    
    while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
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
    

    while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
            end
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
    
        while(1)
        if get(scom, 'BytesAvailable') >= 7 %接收到7个数据
            data_rx = fread(scom, 7, 'char');
            if data_rx(1) == 235 && data_rx(2) == 80 && data_rx(3) == 48
                time_temp = datestr(now, 13);
                set(handles.text31, 'String', num2str(time_temp));
                setappdata(handles.figure1, 'rx_cnt', data_rx(5)+ data_rx(6)*256); 
                set(handles.edit20, 'String', num2str(getappdata(handles.figure1, 'rx_cnt')));
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
