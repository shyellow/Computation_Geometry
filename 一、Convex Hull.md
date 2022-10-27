# 一、Convex Hull

## B.Extreme Points

1. 极性：所有被保留的点都会拥有一条穿过它们的直线，使所有的点都落在它的同一侧
   
   > 如何甄别出极点？
   > 
   > 判断一个点是否会在三个点组成的三角形中

2. In-Triangle-Test
   
   > 伪代码分析：
   > 
   > 标记所有的点都为extreme
   > 
   > 循环每一个三角形（p，q，r）
   > 
   > 对于每一个s都不属于三角形中（p, q, r）
   > 
   > 如果s属于三角形中，则标记s为non_extreme

<img src="file:///G:/markdown_notes/global_images/2022-10-27-19-45-33-image.png" title="" alt="" width="546">

```c
void extremePoint(Point S[], int n) {//n > 2
	for (int s = 0; s < n; s++) S[s].extreme = true;
	for(int p = 0; p < n; p++)
		for(int q = p+1; q < n; q++)
			for(int r = q+1; r < n; r++)
				for (int s = 0; s < n; s++) {
					if (s == p || s == q || s == r || !S[s].extreme)//is extreme
						continue;
					if (InTriangle(S[p], S[q], S[r], S[s]))//in Triangle
						S[s].extreme = false;
				}
}
```

>  **复杂度O($n^4$)**

3. To-Left-Test
   
   > 如何判断一个点是否落在三角形中？
   > 
   > 将In—Triangle-Test转换为三次To-Left-Test，如果一个点在三角形内部，则三次都返回true/false

```c
bool InTriangle(Point p, Point q, Point r, Point s) {
	bool pqLeft = ToLeft(p, q, s);
	bool qrLeft = ToLeft(q, r, s);
	bool rqLeft = ToLeft(r, q, s);
	return (pqLeft == qrLeft) && (qrLeft == rqLeft);//all is true/false
}
```

4. Determinant
   
   （利用两点坐标得出直线方程，由判断点到直线的距离可求出点是否在左侧）
   
   但这种方法太麻烦，采用行列式即可判断，但在这里采用行列式的两倍，如果大于0则说明点在直线左侧。
   
   <img src="file:///G:/markdown_notes/global_images/2022-10-27-19-56-14-image.png" title="" alt="" width="552">

## C.Extreme Edges

1. 极边：对凸包有贡献的边，即所有的点都会落在它的同侧。

2. Algorithm
   
   > 如何判断两个点组成的边为极边？
   > 
   > 如果对于任意一个点s（不属于p，q），都在p，q组成的边的同一侧，则说明pq为极边。

>  **复杂度O($n^3$)**

![](G:\markdown_notes\global_images\2022-10-27-20-04-03-image.png)

```c
void markEE(Point S[], int n) {//n > 2
	for (int k = 0; k < n; k++) {
		S[k].extreme = false;
	}
	for (int p = 0; p < n; p++)
		for (int q = p + 1; q < n; q++)
			checkEdge(S, n, p, q);
}
```

```c
void checkEdge(Point S[], int n, int p, int q) {
	bool LEmpty = true, REmpty = true;
	for (int k = 0; k < n && (LEmpty || REmpty); k++)
		if (k != p && k != q)
			ToLeft(S[p], S[q], S[k]) ? LEmpty = false : REmpty = false;
	if (LEmpty || REmpty)
		S[p].extreme = S[q].extreme = true;
		
}
```

## D.Incremental Construction

1. Decrease and conquer
   
   > Incremental strategy(蚕食策略)——从未排序的部分一点点蚕食
   
   注意：这里是将unsorted插入到sorted中，unsorted的值可能会大于sorted的最小值
   
   ![](G:\markdown_notes\global_images\2022-10-27-20-06-59-image.png)
   
   

> 注意：每增加一个点，可能使极点增加、减少或不变。

2. In-Convex-Polygon-Test
   
   > 判断一个点是否在凸多边形内？
   > 
   > 使用In-Polygon-Test

预处理（本质为排序），模仿有序向量的高效查找算法，如二分查找。

步骤：(1)选一个基点和中点连起来，形成一个有向直线。

（2）做To-Left-Test，在left/right

（3）在相应的区域重复步骤（1）

（4）最终会找到一个三角形，做In-Triangle

![](G:\markdown_notes\global_images\2022-10-27-20-17-33-image.png)

3. Why not Binary Search
   
   ![](G:\markdown_notes\global_images\2022-10-27-20-19-09-image.png)

注意：与InsertionSort类似，有序的Sorted会变，因此需要采用vector类似的结构。

**但可用CCW逆向遍历EE，如果所有EE做To-Left-Test都为True，则在多边形内部。**

4. Support-Lines
   
   > 如何将新加入的点，如果是极点，增加或附着到原先的多边形，使之成为继续使用的结构？
   > 
   > 利用Support-Lines
   
   ![](G:\markdown_notes\global_images\2022-10-27-20-28-51-image.png)

**如果新加的点x在外**，可把多边形分成两段st、ts，则st保留，tx、xs加入。

其中x与t，s形成的直线，称原来凸包的切线为tangents或support line

5. Pattern Of Turns
   
   ![](G:\markdown_notes\global_images\2022-10-27-20-31-51-image.png)
   
   对于任意一个外部的x，可以将凸包上的所有点分为4类

        对x和凸包上的点v确定一条有向直线，对v的前一个前驱点和后继点做To-Left-Test，可将所有点分为四类

| s点  | L+L |
| --- | --- |
| t点  | R+R |
| st  | R+L |
| ts  | L+R |

6. Exterior/Interior
   
   将点在外面和里面的情况结合处理，从任意点出发，对每一个点进行遍历（CCW），每遍历一个点就知道它的pattern，如果在多边形内部，则每个点都有Right/Left；如果在外部则可确定Support-Line，从而重新构建凸包。
   
    

> **复杂度O($n^2$)**

## E. Jarvis March（或GW）

1. SelectionSort
   
   注意：这里是将unsorted的最大值放在sorted的首位，sorted的所有值都大于sorted部分
   
   思想：维护一个局部解，然后找到一个与当前局部解紧密联系的一个解

![](G:\markdown_notes\global_images\2022-10-27-20-43-56-image.png)

2. Strategy
   
   Incremental Construction 的思想：从一个点找到下一个endpoint，再从endpoint开始找到下一个endpoint最终回到起点。

![](G:\markdown_notes\global_images\2022-10-27-20-49-40-image.png)

3. Coherence
   
   > 如何基于现在道已有的极点，沿下一个端点拓展出一条新极边？
   > 
   > 思想：对于最后一条极边所在的直线与剩下的点形成的夹角中，角度最小者即为下一个极点。

![](G:\markdown_notes\global_images\2022-10-27-20-53-20-image.png)

4. To-Left-Test
   
   为了避免计算角度而产生的各种误差，采用to-left-test

        假设k与一个点形成极边，让新点与该边做To-Left-Test，如果新点在左侧，则让k与新点形成极边。

        思想：与选择排序一样，只是把selection中的比较器替换为To-Left-Test

5. Degeneracy
   
   假设ks这条边上不存在另一个点，使夹角一样。

6. Lowest-Then-Leftmost（记作LTL）
   
   思想：将最下面的点作为第一个点，如果有多个，则取最左边的点。假想，第一条边通向x正无穷，如果不是也可以旋转。

7. Implementation
   
   
