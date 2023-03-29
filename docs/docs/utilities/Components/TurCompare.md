---
layout: default
title: TurCompare.h
parent: Utilities
nav_order: 1
---




```cpp
    /**
	 * \brief Compare if 2 clients are the same.
	 * \param aClientOne Is the first client to compare.
	 * \param aClientTwo Is the second client to compare.
	 * \return It return true if the two clients are the same. false if they are not.
	 */
	inline bool CompareClients(sockaddr_in& aClientOne, sockaddr_in& aClientTwo)
	{
		return ((aClientOne.sin_addr.S_un.S_addr == aClientTwo.sin_addr.S_un.S_addr) && (aClientOne.sin_port == aClientTwo.sin_port));
	}
```


