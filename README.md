# 🦍 Donkey Kong (동키콩 모작)

## 🎮 개요
 - 게임 장르: 플랫포머 게임
 - 플랫폼: PC
 - 플레이 인원: 1인
 - 1인 개발
 - **구현 내용**:
   > + **WinAPI를 활용**하여 2D 게임 개발
   > + 스프라이트 애니메이션, 인트로, 엔딩씬 애니메이션 (→ 더블 버퍼링 적용)
   > + **총 5개의 스테이지**(+ 스테이지 별 새로운 장애물들), 망치 아이템
   > + **사운드**(+ 소리 조절 기능-배경음/효과음 개별적으로), 일시정지 기능, 다시시작 기능
 - 개발에 사용된 기술 스택:
   + WinAPI
   + C++
 - 시연 영상: [▶️ 유투브 영상 보기](https://www.youtube.com/)
<img width="500" height="527" alt="image" src="https://github.com/user-attachments/assets/cd3ccedf-d860-4ea3-b62f-f4214c8e241c" />


---

## 📝 구현 내용 상세 설명
   
### 🧱 계단 오르기 알고리즘
 - **부드러운 계단(플랫폼) 상호작용**을 위해 신경을 많이 씀!
 - 플랫폼에 밟고 올라 갈 수 있어야 하며 **동시에** 점프를 하다가 플랫폼에 머리를 부딪힐 수 있도록 만들고자 하였다.
   - 문제 발생: 근데 이때, 사다리를 올라가다가 플랫폼을 다 올라가기 직전에(플랫폼 사이에서) 사다리 옆으로 떨어지거나 상승하는 계단 형태의 플랫폼을 지나가면, 캐릭터를 플랫폼 위로 올려야하나 아니면 아래로 머리를 부딪혀서 아래로 떨어지게 만들어야 하는 지 판단하기 **애매한 상황이 발생**;;
   - 해결 방법:
     - 먼저 캐릭터가 *공중* 에 있는지를 판단한다.
     - 이미 플랫폼 위에 서있는 상태(공중 X)에서 걷다가 다른 플랫폼에 닿을 시 **발의 위치와 해당 충돌 플랫폼의 상단에 위치를 비교**해 '별로 차이가 나지 않는 상황'이라면 '캐릭터가 계단을 오르고 있다고 판단'하여 캐릭터를 해당 플랫폼 위로 올린다.
     - 반면, 공중에 있던 상태인데 플랫폼과 충돌한 상황이라면 **머리의 위치와 해당 충돌 플랫폼의 하단에 위치를 비교**하여 '머리 위치가 플랫폼보다 아래'있었다면 '점프 도중 플랫폼 하단에 머리를 부딪혔다고 판단'하고 캐릭터를 떨어뜨리며, 반대로 '머리 위치가 플랫폼보다 위에 존재'했다면 '캐릭터가 사다리를 타다가 거의 올라온 상태에서 옆으로 떨어진 경우'거나 '플랫폼 사이를 점프해서 지나가다가 다음 플랫폼에 간신히 발/손을 올린 경우라 생각'하고 **천천히 파쿠르**하여 올라간다고 보이도록 플랫폼 위로 올라가게 천천히 캐릭터를 위로 올리도록 만들었다!
       * 참고로, 충돌체크는 WinAPI 내장 함수 'bool IntersectRect'함수를 사용하여 두 사각형(히트박스) 사이에 서로 겹치는 영역이 있는지 파악하여 판단
 * 구현 코드: [플랫폼과 상호작용 처리(물리처리)](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L1174)
   
### 🎨 더블 버퍼링을 이용한 부드러운 스프라이트 애니메이션
 - WinAPI GDI에서 메모리 DC(Back Buffer)를 사용한 더블 버퍼링을 구현하여, 오프스크린에서 모든 렌더링을 처리한 뒤 화면에 블리팅(BitBlt)함으로써 플리커링을 제거하고 부드러운 프레임 갱신을 달성
 - 모든 스프라이트는 종류에 따라 폴더를 구분하여 저장, 관리한다.
 * 구현 코드: [더블 버퍼링 설정 및 화면 그리기 절차](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L696), [캐릭터 스프라이트 이미지 폴더](https://github.com/SeungWon-git/Window-Programming/tree/main/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/character), [사물 스프라이트 이미지 폴더](https://github.com/SeungWon-git/Window-Programming/tree/main/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/object)
   
### 🎞️ 인트로, 엔딩씬 애니메이션 (+ 맵 저장 방식)
 - 인트로 애니메이션은 먼저 동키콩이 공주를 업고 사다리를 올라가고 이후 크게 점프하여 땅을 찍으며 *평탄하던 지형을 기울어지게* 만드는데, 이때 콩의 **애니메이션 프레임**에 숫자를 메겨 시간이 지날 때마다 알맞은 스프라이트 이미지로 전환하여 콩의 애니메이션을 구현하고, 땅을 찍을때 지형이 변하는 것은 **인트로 용 맵 파일들**을 따로 만들어 놓고 해당 맵들을 순서대로 읽어와 전환하는 방식으로 전체 지형이 기울어지는 효과를 만들었다.
   * 참고로, 맵은 타일 형태(2차원 배열)로 저장하며, 각 타일 내 정보가 맵에 구성요소로 이루어져 있다.
 - 엔딩씬에 경우에는 마리오가 마지막 구출 지역에 도착하면 공주가 갇혀있던 감옥이 동키콩 머리 위로 떨어져 폭발과 함께 동키콩은 결국 맵 아래로 떨어지게 되는데, 이 또한 애니메이션 프레임을 사용하여 동키콩의 애니메이션을 적용하고 감옥 플랫폼이 떨어지는 효과는 단순하게 해당 플랫폼을 그리는 y축 좌표를 변화시켜 만들었다.
 * 구현 코드: [동키콩 인트로 애니메이션](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L2386), [맵에 사용되는 구조체](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L92), [맵이 저장되어 있는 폴더](https://github.com/SeungWon-git/Window-Programming/tree/main/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/stage_map)
   
### 🛢️ 장애물 설명
 - 먼저 모든 사물은 공중에 있을 시에 **중력**을 받아 오래 떨어 질 수록 가속도가 붙는다. 다만, 최대 중력 가속도가 존재하듯이 **가속에 한계치**는 정해져 있다.
   * 또한, 너무 큰 편차로 물체를 이동시키면 '지형을 무시하고 통과해서 지나가는 문제'가 발생하므로 이를 피하도록 최대 가속량을 정해두었다.
 - 장애물의 종류는 총 3가지로 '일반 원통, 대형 원통, 새'가 존재한다.
   - **원통 장애물**은 먼저 동키콩이 굴러보낸 방향으로 굴러가기 시작하는데, 이때 **사다리에 닿으면 랜덤하게 사다리를 타고 아래로 떨어진다.** 따라서, 마리오는 사다리 주위에 있을 시에 머리 위로 원통이 떨어지는 일에 주의 해야한다. 
   - 원통 장애물은 굴러가던 도중 벽을 만나거나 공중에서 떨어지다가 다시 플랫폼 위로 떨어지면 원래 진행하고 있던 방향의 *반대 방향* 으로 진행 방향이 바뀐다.
   - 원통 장애물이 굴러가다가 결국 맵의 맨 아래로 떨어져 더이상 화면에 보이지 않게 되면 메모리를 삭제시켜 **메모리를 절약**한다.
   - 일반 원통은 마리오가 점프로 피하거나 망치 아이템을 사용해 파괴 할 수 있지만, '**대형 원통**'은 마리오가 점프로 피하지 못할 정도로 크며 망치 아이템으로도 파괴가 불가능하다. 대신, 사다리를 타고 떨어지지 않기 때문에 **사다리를 이용하여 피하도록 설계**하였다.
     * 참고로, 맵을 진행하다 보면 동키콩이 원통을 **랜덤하게 좌/우**로 떨구기도 하고 **크고 작은 원통을 랜덤**하게 선택하며 떨구기 시작한다.
   - **새 장애물**은 일정 간격으로 플레이어의 x좌표 위로 일직선으로 떨어지며 플레이어로 하여금 **계단 위에서 오래 머무르지 못하도록 강제**한다.
   - **망치 아이템**은 맵 곳곳에 배치되어 있으며, 망치를 먼저 획득해야 사용이 가능하다. 망치를 사용하면 일정시간 동안 마리오로 하여금 장애물들을 파괴할 수 있도록 해주며 일반 원통과 새를 파괴할 수 있다.
   - 다만, 여기서 주의 할 점으로 망치를 휘두르는 스프라이트 이미지는 2개가 존재하는데(망치를 위로 들고 있을때, 망치를 앞으로 내리고 있을때), 이때 **망치가 실제로 장애물에 닿아야지만 장애물을 파괴**한다. 즉, *망치 아이템을 사용 중이라고 무적인 상태가 아니다.* 따라서, 망치를 사용하더라도 장애물을 맞출 수 있도록 유의해야 한다.
     * 참고로, 망치아이템을 사용하면 게임 BGM이 바뀌며, **사용시간이 얼마 남지 않게 되면 이를 알려주기 위해 망치 BGM의 소리 크기가 점점 줄어든다.**
 * 구현 코드: [원통의 굴러가는 물리처리](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L1519), [장애물 충돌체크](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L1614), [망치 충돌체크](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L1914)
  
### 🔊 fmod 사운드 라이브러리를 이용하여 사운드 구현
 - 다양한 배경음, 마리오의 걷기-점프-피격-사다리 타기-망치 아이템 획득, 장애물 파괴 등등 **다양한 곳에서 사운드 이펙트가 적용**함
 - 배경음과 효과음 사운드는 각각 키보드 단축키를 눌러서 **따로 소리 크기 조절 & 음소거 ON/OFF가 가능**하다.
 * 구현 코드: [배경/효과음 크기 조절](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L627), [배경/효과음 음소거 ON/OFF](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L250), [배경음 전환](https://github.com/SeungWon-git/Window-Programming/blob/54f5343ebc8537f2cde1086ec0df0bd1618db02c/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C/donkeykong.cpp#L2051)

  
---

## 📎 기타
- 소스 코드: 👉 [소스 코드](https://github.com/SeungWon-git/Window-Programming/tree/main/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/Donkey%20Kong%20-%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C)
- 빌드(배포) 링크: 🕹️ [직접 플레이 해보기(다운로드)](https://drive.google.com/drive/folders/1uL9mAiS_OpV_C3KvY0j6gDmPN6cm8AyU?usp=sharing)
- 조작키 설명: ⌨️ [조작키](https://github.com/SeungWon-git/Window-Programming/blob/main/%EC%B5%9C%EC%A2%85%20%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%20-%20%EB%8F%99%ED%82%A4%EC%BD%A9/%EB%8F%99%ED%82%A4%EC%BD%A9%20-%20%EB%B0%B0%ED%8F%AC/%EC%A1%B0%EC%9E%91%ED%82%A4%20%EC%84%A4%EB%AA%85.txt)
