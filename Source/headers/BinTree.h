#pragma once

namespace SM2K 
{
	template<typename T = void*>
	class BinTree
	{
	public:
		inline bool Insert(const T& _data)
		{
			if(!root)
			{
				root = new Node{ false, nullptr, nullptr, _data};
				++count;
				return true;
			}

			return _insert(_data, root);
		}
		inline bool Find(const T& _data)
		{
			Node* tmp;
			if (root && (tmp = _find(_data, root)))
				return !tmp->deleted;

			return false;
		}
		inline bool Delete(const T& _data)
		{
			Node* tmp;
			if (root && (tmp = _find(_data, root)))
			{
				tmp->deleted = true;		// Keeping it simple since the data shouldn't be general.
				--count;
				if (count <= 0) Clear(); // Clear the whole tree if we deleted everything.
				return true;
			}

			return false;
		}
		inline void Clear() // Clears all data registered in the tree.
		{
			if(root)
			{
				_delete(root);
				root = nullptr;
				count = 0;
			}
		}	

	private:

		struct Node
		{
			bool deleted;
			Node* left = nullptr;
			Node* right = nullptr;
			T data;
		};

		Node* root = nullptr;
		size_t count = 0;

		inline Node* _find(const T& _data, Node* _node)
		{
			if (_data < _node->data)
			{
				if (_node->left)
					_find(_data, _node->left);
				else return nullptr;
			}
			else if (_data > _node->data)
			{
				if (_node->right)
					_find(_data, _node->right);
				else return nullptr;
			}
			else return _node;
			
		}
		inline bool _insert(const T& _data, Node* _node)
		{
			if(_data < _node->data)
			{
				if (!_node->left)
				{
					_node->left = new Node{ false, nullptr, nullptr, _data };
					++count;
					return true;
				}
				else return _insert(_data, _node->left);
			}
			else
			{
				if (!_node->right)
				{
					if (_data == _node->data) 
					{
						if(_node->deleted) // If it was maked deleted, unmark it. 
						{
							_node->deleted = false;
							++count;
							return true;
						}
						return false; // We dont want duplicates since this structure will mainly be used to varify pointers.
					}
					_node->right = new Node{ false, nullptr, nullptr, _data };
					++count;
					return true;
				}
				else return _insert(_data, _node->right);
			}

			return false;
		}
		inline void _delete(Node* _node)
		{
			if (!_node) return;
			_delete(_node->left);
			_delete(_node->right);
			delete _node;
		}

	};

}
