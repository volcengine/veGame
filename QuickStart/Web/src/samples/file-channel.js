/*
 * @Author: chenzhongsheng
 * @Date: 2023-01-05 18:39:38
 * @Description: Coding something
 */
function createBtn(text, fn){
    const btn = document.createElement('button');
    btn.innerText = text
    btn.onclick = fn
    return btn
}

function initTosFileChannel(veGameInstance){

    const wrapper = document.createElement('span');
    wrapper.style.display = 'block';
    
    const text = document.createElement('span');
    text.innerText = '大文件二期：';
    wrapper.appendChild(text);

    const dom = document.createElement('input');
    dom.setAttribute('type', 'file');
    wrapper.appendChild(dom);

    wrapper.appendChild(createBtn(
        '暂停上传', ()=>{
            veGameInstance.pauseSendFile()
            dom.value=''
        }
    ));

    wrapper.appendChild(createBtn(
        '取消上传', ()=>{veGameInstance.stopSendFile()}
    ));
    wrapper.appendChild(createBtn(
        '暂停下载', ()=>{
            veGameInstance.pauseReceiveFile()
        }
    ));

    wrapper.appendChild(createBtn(
        '取消下载', ()=>{veGameInstance.stopReceiveFile()}
    ));

    wrapper.appendChild(createBtn(
        '清空文件', ()=>{dom.value=''}
    ));

    document.querySelector('.func-area').appendChild(wrapper);

    dom.onchange = (e)=>{
        veGameInstance.startSendFile(e.target.files[0], {
            name: '1.mp4',
            folder: '/sdcard/Download',
        });
    }
    [
        'on-send-file-request-error',
        'on-send-file-error',
        'on-send-file-message-error',
        'on-send-file-progress',
        'on-send-file-done',

        'on-download-file-request-error',
        'on-download-file-message-error',
        'on-download-file-progress',
        'on-download-file-done',
    ].forEach(name=>{

        veGameInstance.on(name, d=>{
            console.warn(name, d);
            if(name === 'on-download-file-done'){
                const a = document.createElement('a');
                a.download='ddd.mp4';
                document.body.append(a);
                a.href = URL.createObjectURL(d);
                a.click();
            }
        });
    })
}