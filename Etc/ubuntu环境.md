## Ubuntu 环境

#### 关闭 software update：

在 `software&update` 中 `updates` 将`automatically check for updates` 改为`never`；`notify me of a new ubuntu version` 改为 `never`



#### 关闭 system program problem detected：

```sh
vim /etc/default/apport
```

把 `enabled` 改为 0 即可



#### 关闭 ubuntu 动画：

```sh
gsettings set org.gnome.desktop.interface enable-animations false
```



#### 安装 chrome：

在要安装的目录中执行：

```sh
wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
sudo apt install ./google-chrome-stable_current_amd64.deb
```



## vscode 环境

#### 定时保存：

`File`  - `Preferences` - `Settings` , `auto save` 选择 `afterDelay` 指定毫秒数后保存，下面设置毫秒数



