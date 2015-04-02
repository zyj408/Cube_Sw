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

% Last Modified by GUIDE v2.5 05-Sep-2014 21:06:50

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


% --- Executes just before untitled is made visible.
function untitled_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to untitled (see VARARGIN)

% Choose default command line output for untitled
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes untitled wait for user response (see UIRESUME)
% uiwait(handles.figure1);


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


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
if get(hObject, 'value')
    com_n = sprintf('com%d',get(handles.popupmenu1, 'value'));
    scom = serial(com_n);
    set(scom, 'BaudRate', 115200, 'Parity', 'none', 'DataBits',...
        8, 'StopBits', 1,  'BytesAvailableFcnCount', 120,...
        'BytesAvailableFcnMode', 'byte', 'BytesAvailableFcn', {@uart_rcv,handles});
    set(handles.figure1, 'UserData', scom);
    
    try
        fopen(scom);
    catch
        msgbox('串口无法打开！');
        set(hObject, 'string', '关闭串口');
        set(hObject, 'value', 0);
        return;
    end
     set(hObject, 'string', '关闭串口');
else
    t = timerfind;
    try
        stop(t);
        delete(t);
        clear t
    end
    
    scoms = instrfind;
    stopasync(scoms);
    fclose(scoms);
    delete(scoms);
    set(hObject, 'string', '打开串口');
end

function uart_rcv(obj, event, handles)

n = get(obj, 'BytesAvailable');
if n
    a = fread(obj, n, 'char');
    if (a(1) == 17 && a(2) == 34 && a(119) == 254 && a(120) == 255)
    hh = a(3);
    mm = a(4);
    ss = a(5);
    set(handles.loc_time, 'string', ['本地时间：' datestr(now, 13)]);
    set(handles.sate_time, 'string', ['星上时间：' num2str(hh) ':' num2str(mm) ':' num2str(ss)]);
    
    tel_addr = a(6)*16777216 + a(7)*65536 + a(8)*256 + a(9);
    gps_addr = a(10)*16777216 + a(11)*65536 + a(12)*256 + a(13);
    pld_addr = a(14)*16777216 + a(15)*65536 + a(16)*256 + a(17);
    tel_over_range = a(18);
    gps_over_range = a(19);
    pld_over_range = a(20);
    set(handles.tel_cur_addr, 'string', ['遥测地址：' num2str(tel_addr)]);
    set(handles.gps_cur_addr, 'string', ['GPS地址：' num2str(gps_addr)]);
    set(handles.pld_cur_addr, 'string', ['载荷地址：' num2str(pld_addr)]);
    set(handles.tel_addr_o, 'string', ['遥测地址越界：' num2str(tel_over_range)]);
    set(handles.gps_addr_o, 'string', ['GPS地址越界：' num2str(gps_over_range)]);
    set(handles.pld_addr_o, 'string', ['载荷地址越界：' num2str(pld_over_range)]);
    
    
    
    flash_error = a(27);
    set(handles.flash_check, 'string', ['FLASH监测错误计数：' num2str(flash_error)]);
    
    %%保存数据至TXT
    %%fid = fopen('data\ad_rcd.txt', 'a+');
    %%fprintf(fid,'%d \n',buf1);
    %%fclose(fid);
    %%fid = fopen('data\temp_rcd.txt', 'a+');
    %%fprintf(fid,'%d \n',buf2);
    %%fclose(fid);
    %%fid = fopen('data\vol_rcd.txt', 'a+');
    %%fprintf(fid,'%d \n',buf3);
    %%fclose(fid);
    ad_buf1 = a(21)*256+a(22);
    ad_buf2 = a(23)*256+a(24);
    ad_buf3 = a(25)*256+a(26);
    ad_buf4 = a(27)*256+a(28);
    ad_buf5 = a(29)*256+a(30);
    ad_buf6 = a(31)*256+a(32);
    ad_buf7 = a(33)*256+a(34);
    ad_buf8 = a(35)*256+a(36);
    ad_buf9 = a(37)*256+a(38);
    ad_buf10 = a(39)*256+a(40);
    ad_buf11 = a(41)*256+a(42);
    ad_buf12 = a(43)*256+a(44);
    ad_buf13 = a(45)*256+a(46);
    ad_buf14 = a(47)*256+a(48);
    ad_buf15 = a(49)*256+a(50);
    ad_buf16 = a(51)*256+a(52);
    
    b1 = pushfifo(ad_buf1,1); 
    b2 = pushfifo(ad_buf2,2); 
    b3 = pushfifo(ad_buf3,3); 
    b4 = pushfifo(ad_buf4,4); 
    b5 = pushfifo(ad_buf5,5); 
    b6 = pushfifo(ad_buf6,6); 
    b7 = pushfifo(ad_buf7,7); 
    b8 = pushfifo(ad_buf8,8); 
    b9 = pushfifo(ad_buf9,9); 
    b10 = pushfifo(ad_buf10,10); 
    b11 = pushfifo(ad_buf11,11); 
    b12 = pushfifo(ad_buf12,12); 
    b13 = pushfifo(ad_buf13,13); 
    b14 = pushfifo(ad_buf14,14); 
    b15 = pushfifo(ad_buf15,15); 
    b16 = pushfifo(ad_buf16,16); 

    set(handles.ad_val1, 'string', ['AD采样值：' num2str(ad_buf1)]);
    set(handles.ad_val2, 'string', ['AD采样值：' num2str(ad_buf2)]);
    set(handles.ad_val3, 'string', ['AD采样值：' num2str(ad_buf3)]);
    set(handles.ad_val4, 'string', ['AD采样值：' num2str(ad_buf4)]);
    set(handles.ad_val5, 'string', ['AD采样值：' num2str(ad_buf5)]);
    set(handles.ad_val6, 'string', ['AD采样值：' num2str(ad_buf6)]);
    set(handles.ad_val7, 'string', ['AD采样值：' num2str(ad_buf7)]);
    set(handles.ad_val8, 'string', ['AD采样值：' num2str(ad_buf8)]);
    set(handles.ad_val9, 'string', ['AD采样值：' num2str(ad_buf9)]);
    set(handles.ad_val10, 'string', ['AD采样值：' num2str(ad_buf10)]);
    set(handles.ad_val11, 'string', ['AD采样值：' num2str(ad_buf11)]);
    set(handles.ad_val12, 'string', ['AD采样值：' num2str(ad_buf12)]);
    set(handles.ad_val13, 'string', ['AD采样值：' num2str(ad_buf13)]);
    set(handles.ad_val14, 'string', ['AD采样值：' num2str(ad_buf14)]);
    set(handles.ad_val15, 'string', ['AD采样值：' num2str(ad_buf15)]);
    set(handles.ad_val16, 'string', ['AD采样值：' num2str(ad_buf16)]);
    %%Matlab绘制函数图形
    plot(handles.axes1,b1);
    set(handles.axes1, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes2,b2);
    set(handles.axes2, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes3,b3);
    set(handles.axes3, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes4,b4);
    set(handles.axes4, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes5,b5);
    set(handles.axes5, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes6,b6);
    set(handles.axes6, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes7,b7);
    set(handles.axes7, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes8,b8);
    set(handles.axes8, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes9,b9);
    set(handles.axes9, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);    
    plot(handles.axes10,b10);
    set(handles.axes10, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes11,b11);
    set(handles.axes11, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes12,b12);
    set(handles.axes12, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes13,b13);
    set(handles.axes13, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes14,b14);
    set(handles.axes14, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes15,b15);
    set(handles.axes15, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);
    plot(handles.axes16,b16);
    set(handles.axes16, 'xgrid', 'on', 'ygrid', 'on', 'ylim', [0 4200]);  
  
    else
        c = get(handles.edit2, 'string');
        set(handles.edit2, 'string', num2str(str2num(c) + 1));
    end
    
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
