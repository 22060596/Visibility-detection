# Visibility-detection
Visibility detection based on surveillance image
基于监控图像的能见度检测算法，不定期更新科研项目进展与检测效果，但目前尚未打算公开核心代码  
一、日间检测  
我们基于大气模型与暗通道先验理论设计并实现了一种基于监控图像的能见度检测算法。该算法能够利用待测图像以及待测场景的深度图进行能见度检测。实验表明，该算法在单一场景下的能见度检测精度与自动气象站的能见仪检测精度相当，并具有更好的稳定性，与实际观察更符合（因为能见仪对雾的不均匀性较敏感）。对于不同场景，不需要专门的训练，就能够实现能见度检测，具有较好的推广性。  
在高速公路的能见度检测中，我们较为关心的是低能见度时检测的准确性。因此，我们针对余姚、舟山两个观测点，对气象站能见度数据低于2km的图像做了测试。下图是余姚及舟山测试点位的图像示例。  
![image](https://user-images.githubusercontent.com/76093504/156872287-422ef81c-8a00-426e-bce1-85826b8a1c06.png)![image](https://user-images.githubusercontent.com/76093504/156872299-9348a978-df77-45dc-9fa1-342d38e0d8d9.png)  
图1 余姚（左）和舟山（右）测试点图像示例  
（1）余姚测试点  
首先，我们利用余姚测试点来验证该算法在单一场景下能见度检测的准确性。测试数据为杭甬高速K266+850甬向（余姚）监控相机所记录的能见度低于2km的图片，合计208张。  
![image](https://user-images.githubusercontent.com/76093504/156872331-ed649aa0-a2b6-4381-b795-89b3efb53b51.png)  
上图展示了余姚观测点208张图片的检测结果。横轴坐标代表以气象站能见度仪测得的能见度值，纵轴坐标为本项目基于图像的检测算法所计算出的能见度值。三条直线分别为y=1.1x，y=x和y=0.9x。数据点离y=x直线越近，则表示计算出的结果越接近。总体误差分布见下表：  
![image](https://user-images.githubusercontent.com/76093504/156872393-3ed7314c-dd26-4b4b-b1c9-9f521010e713.png)  
其中，126张图片检测误差小于10%，197张图片检测误差小于20%。尽管误差小于10%的比例并不算高，但误差小于20%的比例达到了94.7%。  
一方面，由于气象站使用能见度检测仪进行能见度测量，其本身会带有一定的误差，一旦能见度仪的偏差方向与检测算法的偏差方向相反，就会带来更大的偏差。另一方面，考虑到监控相机与能见度仪检测点之间存在一定的距离，而雾又具有较强的局部性，所以能见度仪检测出的值与监控相机所拍摄画面的能见度值之间会不一致。以上两点因素会造成真值标签的不精确。因此选择误差20%作为算法的评估指标可能更为合理。  
（2）舟山测试点  
其次，我们利用舟山测试点的数据来验证该算法的是否具有推广性。  
![image](https://user-images.githubusercontent.com/76093504/156872413-5aa2f37e-9cbf-4f4f-8fc2-b3c5f31b0b66.png)  
图3 舟山能见度检测情况  
上图是舟山测试点的能见度检测结果的大体情况。图像数据的能见度分布较为均匀，总体检测结果也较为准确。  
![image](https://user-images.githubusercontent.com/76093504/156872514-179edb4c-dc7a-4fc1-86c4-951111a3223e.png)
在总计218张测试图片中，152张图片检测误差小于10%，207张图片检测误差小于20%。说明上述模型在不同场景下也具有很好的检测能力，具有较好的推广性。  
（3）牟山测试点（能见度数据来自浙江气象中心）  
我们利用气象中心的数据来进一步验证算法的推广性。由于牟山点位观测到了较多低能见度的图像，因此我们使用牟山观测点作为气象中心的测试点。  
![image](https://user-images.githubusercontent.com/76093504/156872566-be37bba2-a09d-49ad-bbc1-0541bc8d5a9c.png)  
图4 牟山测试点图像示例  
![image](https://user-images.githubusercontent.com/76093504/156872578-94ad4f3c-7b95-4775-ad94-163125cfda98.png)  
图5 舟山能见度检测情况  
上图是牟山测试点的能见度检测结果的大体情况。低能见度数据分布较为密集，总体检测结果也较为准确。  
![image](https://user-images.githubusercontent.com/76093504/156872614-ba4ffd01-fe50-4263-97f6-03f19b89faec.png)  
总计209张测试图片中，106张图片检测误差小于10%，172张图片检测误差小于20%，相较于余姚与舟山两个测试点的精度有所下降。但考虑到气象中心的能见度数据来自于邻近的气象站，与摄像机有较大的距离，因此来自气象中心的能见度数据与摄像机所在地的真实能见度之间会存在较大的误差。考虑到这点，可以认为我们的算法应该具有较高的准确度。  
![image](https://user-images.githubusercontent.com/76093504/156872623-1e50d973-e484-4dde-bc59-8b140520ae9a.png)  
图6中可以看到，高能见度时二者相差较大，低能见度时二者吻合较好；能见度低于2km的部分主要集中在第1000至1500个测试点以及第2500个测试点附近，时间上分别对应2月12日以及2月14日。从图像上看，如图7所示，这两天的确为有雾天气，其中，2月12日我们的图像检测算法与能见度仪的检测结果吻合的较好，但2月14日二者偏差较大，算法给出的结果更符合实际情况。黑色数字为能见度仪所检测出的能见度值（单位米），红色字体则是算法检测出的能见度值。显然，图像检测算法的计算结果比能见度仪更加接近实际的能见度值，而这种这情况在其余测试点和其他时间段也都有发生。  
我们分析，可能是出于雾的局部特性，能见度仪在许多时间点未能检测出的低能见度信息，造成了在某些情况下检测结果不够稳定。而我们的算法却较好地避免了这种现象，能够持续地检测出该日早晨的雾气。  
在能见度检测仪的检测结果也会与实际能见度值产生偏差的情况下，我们认为该图6所展示的算法检测结果，在整体上能够较好地匹配实际能见度的变化趋势。  
![image](https://user-images.githubusercontent.com/76093504/156872635-7fd24dcf-bc0c-4e1b-956b-fb9dee720b16.png)![image](https://user-images.githubusercontent.com/76093504/156872637-1f019ba9-3dd4-451d-9b5e-7490351a77a2.png)  
图7 余姚点位截图

二、夜间检测  






